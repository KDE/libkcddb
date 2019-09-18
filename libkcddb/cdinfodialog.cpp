/*
  Copyright (c) 2005 Shaheedur R. Haque <srhaque@iee.org>
  Copyright (C) 2005 Richard Lärkäng <nouseforaname@home.se>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#include "cdinfodialog.h"

#include "cdinfoencodingwidget.h"
#include "kcddbi18n.h"
#include "ui_cdinfodialog.h"

#include <KCodecs/KCharsets>

#include <QDebug>
#include <QStringList>
#include <QTextCodec>
#include <QTime>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QDialogButtonBox>

using KCDDB::TrackInfo;

namespace KCDDB
{

class CDInfoDialog::Private
{
  public:
    Private()
        : ui(new Ui::CDInfoDialogBase)
    {
    }

    CDInfo info;
    KCDDB::Genres genres;
    KCDDB::Categories categories;
    Ui::CDInfoDialogBase* ui;
    static QLatin1String SEPARATOR;
    static const unsigned TRACK_NUMBER = 0;
    static const unsigned TRACK_TIME = 1;
    static const unsigned TRACK_TITLE = 2;
    static const unsigned TRACK_ARTIST = 3;
    static const unsigned TRACK_COMMENT = 4;
};

  QLatin1String CDInfoDialog::Private::SEPARATOR = QLatin1String( " / " );

  CDInfoDialog::CDInfoDialog(QWidget* parent)
    : QDialog(parent),
      d(new Private)
  {
      QWidget* w = new QWidget(this);
      d->ui->setupUi(w);

      QVBoxLayout* layout = new QVBoxLayout(this);
      layout->addWidget(w);

      d->info.set(QLatin1String( "source" ), QLatin1String( "user" ));

      d->categories = KCDDB::Categories();
      d->ui->m_category->addItems(d->categories.i18nList());
      d->genres = KCDDB::Genres();
      d->ui->m_genre->addItems(d->genres.i18nList());

      m_trackModel = new QStandardItemModel(0, 5, this);
      d->ui->m_trackList->setModel(m_trackModel);

      // We want control over the visibility of this column. See artistChanged().
      d->ui->m_trackList->header()->setSectionResizeMode(Private::TRACK_ARTIST, QHeaderView::Interactive);

      connect( d->ui->m_trackList, SIGNAL(activated(QModelIndex)), this, SLOT(slotTrackSelected(QModelIndex)) );
      connect( d->ui->m_trackList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotTrackDoubleClicked(QModelIndex)) );
      connect( d->ui->m_artist, SIGNAL(textChanged(QString)), this, SLOT(artistChanged(QString)) );
      connect( d->ui->m_genre, SIGNAL(textChanged(QString)), this, SLOT(genreChanged(QString)) );
      connect( d->ui->m_multiple, SIGNAL(toggled(bool)), this, SLOT(slotMultipleArtists(bool)) );

      connect(d->ui->m_changeEncoding,SIGNAL(clicked()),SLOT(slotChangeEncoding()));

      QDialogButtonBox *bbox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
      layout->addWidget(bbox);
      connect(bbox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), SLOT(accept()));
      connect(bbox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), SLOT(close()));
  }

  void CDInfoDialog::slotTrackSelected( const QModelIndex &index )
  {
      emit play(index.data().toUInt()-1);
  }


  CDInfoDialog::~CDInfoDialog()
  {
      delete d->ui;
      delete d;
  }

  void CDInfoDialog::slotNextTrack()
  {
      QModelIndex index = d->ui->m_trackList->indexBelow(d->ui->m_trackList->currentIndex());
      if (index.isValid())
      {
          d->ui->m_trackList->setCurrentIndex(index);
          d->ui->m_trackList->scrollTo(index, QTreeView::EnsureVisible);
      }
  }

  void CDInfoDialog::slotTrackDoubleClicked(const QModelIndex &index)
  {
      d->ui->m_trackList->edit(index);
  }

  void CDInfoDialog::setInfo( const KCDDB::CDInfo &info, const KCDDB::TrackOffsetList &trackStartFrames )
  {
      d->info = info;

      d->ui->m_artist->setText(info.get(Artist).toString().trimmed());
      d->ui->m_title->setText(info.get(Title).toString().trimmed());
      d->ui->m_category->setItemText(d->ui->m_category->currentIndex(), d->categories.cddb2i18n(info.get(Category).toString()));

      // Make sure the revision is set before the genre to allow the genreChanged() handler to fire.
      d->ui->m_revision->setText(QString::number(info.get(QLatin1String( "revision" )).toInt()));
      d->ui->m_genre->setItemText(d->ui->m_genre->currentIndex(), d->genres.cddb2i18n(info.get(Genre).toString()));
      d->ui->m_year->setValue(info.get(Year).toInt());
      d->ui->m_comment->setText(info.get(Comment).toString().trimmed());
      d->ui->m_id->setText(info.get(QLatin1String( "discid" )).toString().trimmed());

      // Now do the individual tracks.
      unsigned tracks = info.numberOfTracks();
      if (tracks > 0)
      {
         d->ui->m_length->setText(framesTime(trackStartFrames[tracks] - trackStartFrames[0]));
      }

      m_trackModel->clear();
      QStringList labels;
      labels << i18n("Track") << i18n("Length") << i18n("Title") << i18n("Artist") << i18n("Comment");
      m_trackModel->setHorizontalHeaderLabels(labels);
      for (unsigned i = 0; i < tracks; ++i) {
          QList<QStandardItem *> trackItems = QList<QStandardItem *>();
          TrackInfo ti(info.track(i));
          QStandardItem *trackNumberItem = new QStandardItem(QString::asprintf("%02d", i + 1));
		  trackNumberItem->setEditable(false);
          trackItems << trackNumberItem;
          QStandardItem *trackLengthItem = new QStandardItem(framesTime(trackStartFrames[i + 1] - trackStartFrames[i]));
		  trackLengthItem->setEditable(false);
          trackItems << trackLengthItem;
          QStandardItem *trackTitleItem = new QStandardItem(ti.get(Title).toString());
          trackItems << trackTitleItem;
          QStandardItem *trackArtistItem = new QStandardItem(ti.get(Artist).toString());
          trackItems << trackArtistItem;
          QStandardItem *trackCommentItem = new QStandardItem(ti.get(Comment).toString());
          trackItems << trackCommentItem;

          m_trackModel->appendRow(trackItems);
      }
      // FIXME KDE4: handle playorder here too, once KCDDBInfo::CDInfo is updated.

      if (info.get(Artist).toString() == QLatin1String( "Various" ) || d->ui->m_multiple->isChecked()){
          d->ui->m_trackList->resizeColumnToContents(Private::TRACK_ARTIST);
    }
  }

  QString CDInfoDialog::framesTime(unsigned frames)
  {
      QTime time;
      double ms;

      ms = frames * 1000 / 75.0;
      time = time.addMSecs((int)ms);

      // Use ".zzz" for milliseconds...
      QString temp2;
      if (time.hour() > 0)
          temp2 = time.toString(QLatin1String( "hh:mm:ss" ));
      else
          temp2 = time.toString(QLatin1String( "mm:ss" ));
      return temp2;
  } // framesTime

  KCDDB::CDInfo CDInfoDialog::info() const
  {
      KCDDB::CDInfo info = d->info;

      info.set(Artist, d->ui->m_artist->text().trimmed());
      info.set(Title, d->ui->m_title->text().trimmed());
      info.set(Category, d->categories.i18n2cddb(d->ui->m_category->currentText()));
      info.set(Genre, d->genres.i18n2cddb(d->ui->m_genre->currentText()));
      info.set(Year, d->ui->m_year->value());
      info.set(Comment, d->ui->m_comment->text().trimmed());
      info.set(QLatin1String( "revision" ), d->ui->m_revision->text().trimmed().toUInt());
      info.set(QLatin1String( "discid" ), d->ui->m_id->text().trimmed());

    for (int t = 0; t < m_trackModel->rowCount(); ++t) {
          TrackInfo& track = info.track(t);
          track.set(Artist, m_trackModel->index(t, Private::TRACK_ARTIST).data().toString().trimmed());
          track.set(Title, m_trackModel->index(t, Private::TRACK_TITLE).data().toString().trimmed());
          track.set(Comment, m_trackModel->index(t, Private::TRACK_COMMENT).data().toString().trimmed());
          // FIXME KDE4: handle track lengths here too, once KCDDBInfo::CDInfo is updated.
      }
      // FIXME KDE4: handle playorder here too, once KCDDBInfo::CDInfo is updated.
      return info;
  }


  void CDInfoDialog::artistChanged( const QString &newArtist )
  {
      // Enable special handling of compilations.
      if (newArtist.trimmed().compare(QLatin1String( "Various" ))) {
          d->ui->m_multiple->setChecked(false);
      } else {
          d->ui->m_multiple->setChecked(true);
      }
  }

  void CDInfoDialog::genreChanged( const QString &newGenre )
  {
      // Disable changes to category if the version number indicates that a record
      // is already in the database, or if the genre is poorly set. The latter
      // condition also provides a "back-door" override.
      d->ui->m_category->setEnabled((d->ui->m_revision->text().trimmed().toUInt() < 1) ||
                              (newGenre.compare(QLatin1String( "Unknown" )) == 0));
  }


  void CDInfoDialog::slotMultipleArtists( bool hasMultipleArtist)
  {
      if (hasMultipleArtist) {
        for (int t = 0; t < m_trackModel->rowCount(); ++t) {
              QString title = m_trackModel->index(t, Private::TRACK_TITLE).data().toString();
              int separator = title.indexOf(Private::SEPARATOR);
              if (separator != -1) {
                  // Artists probably entered already
                  m_trackModel->setData(m_trackModel->index(t, Private::TRACK_ARTIST), title.left(separator));
                  m_trackModel->setData(m_trackModel->index(t, Private::TRACK_TITLE), title.mid(separator + 3));
              }
          }
          d->ui->m_trackList->resizeColumnToContents(Private::TRACK_ARTIST);
          d->ui->m_trackList->resizeColumnToContents(Private::TRACK_TITLE);
      } else {
        for (int t = 0; t < m_trackModel->rowCount(); ++t) {
              QString artist = m_trackModel->index(t, Private::TRACK_ARTIST).data().toString();
              if (!artist.isEmpty()) {
                  m_trackModel->setData(m_trackModel->index(t, Private::TRACK_ARTIST), QString());
                  QString title = m_trackModel->index(t, Private::TRACK_TITLE).data().toString();
                  m_trackModel->setData(m_trackModel->index(t, Private::TRACK_TITLE), QVariant(artist + Private::SEPARATOR + title));
              }
      }
      d->ui->m_trackList->hideColumn(Private::TRACK_ARTIST);
      d->ui->m_trackList->resizeColumnToContents(Private::TRACK_TITLE);
  }
}


  void CDInfoDialog::slotChangeEncoding()
  {
      QDialog* dialog = new QDialog(this);
      dialog->setWindowTitle(i18n("Change Encoding"));
      dialog->setModal( true );


      QStringList songTitles;
        for (int t = 0; t < m_trackModel->rowCount(); ++t) {
              QString title = m_trackModel->index(t, Private::TRACK_ARTIST).data().toString().trimmed();
              if (!title.isEmpty())
              title.append(Private::SEPARATOR);
              title.append(m_trackModel->index(t, Private::TRACK_TITLE).data().toString().trimmed());
              songTitles << title;
          }

      KCDDB::CDInfoEncodingWidget* encWidget = new KCDDB::CDInfoEncodingWidget(
          dialog, d->ui->m_artist->text(),d->ui->m_title->text(), songTitles);

      QVBoxLayout* layout = new QVBoxLayout(dialog);
      layout->addWidget(encWidget);

      if (dialog->exec())
      {
        KCharsets* charsets = KCharsets::charsets();
        QTextCodec* codec = charsets->codecForName(charsets->encodingForName(encWidget->selectedEncoding()));

        d->ui->m_artist->setText(codec->toUnicode(d->ui->m_artist->text().toLatin1()));
        d->ui->m_title->setText(codec->toUnicode(d->ui->m_title->text().toLatin1()));
        d->ui->m_genre->setItemText(d->ui->m_genre->currentIndex(), codec->toUnicode(d->ui->m_genre->currentText().toLatin1()));
        d->ui->m_comment->setText(codec->toUnicode(d->ui->m_comment->text().toLatin1()));

        QModelIndex trackIndex = m_trackModel->index(0, 0, QModelIndex());
        int trackRows = m_trackModel->rowCount(trackIndex);
        for (int t = 0; t < trackRows; ++t) {
            QString artist = m_trackModel->index(t, Private::TRACK_ARTIST, trackIndex).data().toString();
            m_trackModel->setData(m_trackModel->index(t, Private::TRACK_ARTIST, trackIndex), codec->toUnicode(artist.toLatin1()));
            QString title = m_trackModel->index(t, Private::TRACK_TITLE, trackIndex).data().toString();
            m_trackModel->setData(m_trackModel->index(t, Private::TRACK_TITLE, trackIndex), codec->toUnicode(title.toLatin1()));
            QString comment = m_trackModel->index(t, Private::TRACK_COMMENT, trackIndex).data().toString();
            m_trackModel->setData(m_trackModel->index(t, Private::TRACK_COMMENT, trackIndex), codec->toUnicode(comment.toLatin1()));
        }
      }
  }
}


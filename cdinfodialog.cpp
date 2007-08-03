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
#include "ui_cdinfodialog.h"

#include <qtextcodec.h>
#include <kdebug.h>
#include <QDateTime>
#include <kglobal.h>
#include <kcharsets.h>

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
    static const char *SEPARATOR;
    static const unsigned TRACK_TIME = 1;
    static const unsigned TRACK_NUMBER = 0;
    static const unsigned TRACK_TITLE = 2;
    static const unsigned TRACK_COMMENT = 3;
    static const unsigned TRACK_ARTIST = 4;
};

  const char *CDInfoDialog::Private::SEPARATOR = " / ";

  CDInfoDialog::CDInfoDialog(QWidget* parent)
    : KDialog(parent),
      d(new Private)
  {
      QWidget* w = new QWidget(this);
      d->ui->setupUi(w);
      setMainWidget(w);

      d->info.set("source", "user");

      d->categories = KCDDB::Categories();
      d->ui->m_category->addItems(d->categories.i18nList());
      d->genres = KCDDB::Genres();
      d->ui->m_genre->addItems(d->genres.i18nList());

      d->ui->m_trackList->addColumn(i18n("Track"));
      d->ui->m_trackList->addColumn(i18n("Length"));
      d->ui->m_trackList->addColumn(i18n("Title"));
      d->ui->m_trackList->addColumn(i18n("Comment"));
      d->ui->m_trackList->addColumn(i18n("Artist"));

      // We want control over the visibility of this column. See artistChanged().
      d->ui->m_trackList->setColumnWidthMode(Private::TRACK_ARTIST, Q3ListView::Manual);

      // Make the user-definable values in-place editable.
      d->ui->m_trackList->setRenameable(Private::TRACK_NUMBER, false);
      d->ui->m_trackList->setRenameable(Private::TRACK_TIME, false);
      d->ui->m_trackList->setRenameable(Private::TRACK_TITLE, true);
      d->ui->m_trackList->setRenameable(Private::TRACK_COMMENT, true);
      d->ui->m_trackList->setRenameable(Private::TRACK_ARTIST, true);

      // ensure we get our translations
      KGlobal::locale()->insertCatalog("libkcddb");
      connect( d->ui->m_trackList, SIGNAL( selectionChanged(Q3ListViewItem*) ), this, SLOT( slotTrackSelected(Q3ListViewItem*) ) );
      connect( d->ui->m_trackList, SIGNAL( doubleClicked(Q3ListViewItem*,const QPoint&,int) ), this, SLOT( slotTrackDoubleClicked(Q3ListViewItem*,const QPoint&,int) ) );
      connect( d->ui->m_artist, SIGNAL( textChanged(const QString&) ), this, SLOT( artistChanged(const QString&) ) );
      connect( d->ui->m_genre, SIGNAL( textChanged(const QString&) ), this, SLOT( genreChanged(const QString&) ) );
      connect( d->ui->m_multiple, SIGNAL( toggled(bool) ), this, SLOT( slotMultipleArtists(bool) ) );

      connect(d->ui->m_changeEncoding,SIGNAL(clicked()),SLOT(slotChangeEncoding()));
  }

  void CDInfoDialog::slotTrackSelected( Q3ListViewItem *item )
  {
      emit play(item->text(0).toUInt()-1);
  }

  void CDInfoDialog::slotNextTrack()
  {
      if (d->ui->m_trackList->currentItem())
      {
          Q3ListViewItem *item = d->ui->m_trackList->currentItem()->nextSibling();
          d->ui->m_trackList->setSelected(item, true);
          d->ui->m_trackList->ensureItemVisible(item);
      }
  }

  void CDInfoDialog::slotTrackDoubleClicked( Q3ListViewItem *item, const QPoint &, int column)
  {
      d->ui->m_trackList->rename(item, column);
  }

  void CDInfoDialog::setInfo( const KCDDB::CDInfo &info, const KCDDB::TrackOffsetList &trackStartFrames )
  {
      d->info = info;

      d->ui->m_artist->setText(info.get(Artist).toString().trimmed());
      d->ui->m_title->setText(info.get(Title).toString().trimmed());
      d->ui->m_category->setItemText(d->ui->m_category->currentIndex(), d->categories.cddb2i18n(info.get(Category).toString()));

      // Make sure the revision is set before the genre to allow the genreChanged() handler to fire.
      d->ui->m_revision->setText(QString::number(info.get("revision").toInt()));
      d->ui->m_genre->setItemText(d->ui->m_genre->currentIndex(), d->genres.cddb2i18n(info.get(Genre).toString()));
      d->ui->m_year->setValue(info.get(Year).toInt());
      d->ui->m_comment->setText(info.get(Comment).toString().trimmed());
      d->ui->m_id->setText(info.get("discid").toString().trimmed());

      // Now do the individual tracks.
      unsigned tracks = info.numberOfTracks();
      if (tracks > 0)
      {
         d->ui->m_length->setText(framesTime(trackStartFrames[tracks] - trackStartFrames[0]));
      }

      d->ui->m_trackList->clear();
      for (unsigned i = 0; i < tracks; i++)
      {
          Q3ListViewItem *item = new Q3ListViewItem(d->ui->m_trackList, 0);

          TrackInfo ti(info.track(i));

          item->setText(Private::TRACK_NUMBER, QString().sprintf("%02d", i + 1));
          item->setText(Private::TRACK_TIME, framesTime(trackStartFrames[i + 1] - trackStartFrames[i]));
          item->setText(Private::TRACK_ARTIST, ti.get(Artist).toString());
          item->setText(Private::TRACK_TITLE, ti.get(Title).toString());
          item->setText(Private::TRACK_COMMENT, ti.get(Comment).toString());
      }
      // FIXME KDE4: handle playorder here too, once KCDDBInfo::CDInfo is updated.

      if (info.get(Artist).toString() == "Various" || d->ui->m_multiple->isChecked()){
          d->ui->m_trackList->adjustColumn(Private::TRACK_ARTIST);
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
          temp2 = time.toString("hh:mm:ss");
      else
          temp2 = time.toString("mm:ss");
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
      info.set("revision", d->ui->m_revision->text().trimmed().toUInt());
      info.set("discid", d->ui->m_id->text().trimmed());
      int i=0;
      for (Q3ListViewItem *item = d->ui->m_trackList->firstChild(); item; item=item->nextSibling())
      {
          TrackInfo& track = info.track(i);
          track.set(Artist,item->text(Private::TRACK_ARTIST).trimmed());
          track.set(Title,item->text(Private::TRACK_TITLE).trimmed());
          track.set(Comment,item->text(Private::TRACK_COMMENT).trimmed());
          i++;
          // FIXME KDE4: handle track lengths here too, once KCDDBInfo::CDInfo is updated.
      }
      // FIXME KDE4: handle playorder here too, once KCDDBInfo::CDInfo is updated.
      return info;
  }


  void CDInfoDialog::artistChanged( const QString &newArtist )
  {
      // Enable special handling of compilations.
      if (newArtist.trimmed().compare("Various")) {
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
                              (newGenre.compare("Unknown") == 0));
  }


  void CDInfoDialog::slotMultipleArtists( bool hasMultipleArtist)
  {
      if(hasMultipleArtist){
          for (Q3ListViewItem *item = d->ui->m_trackList->firstChild(); item; item=item->nextSibling())
          {
              QString title = item->text(Private::TRACK_TITLE);
          int separator = title.indexOf(Private::SEPARATOR);
          if (separator != -1)
          {
              // Artists probably entered already
              item->setText(Private::TRACK_ARTIST, title.left(separator));
              item->setText(Private::TRACK_TITLE, title.mid(separator + 3));
          }
      }
      d->ui->m_trackList->adjustColumn(Private::TRACK_ARTIST);
      d->ui->m_trackList->adjustColumn(Private::TRACK_TITLE);
  }
  else{
      for (Q3ListViewItem *item = d->ui->m_trackList->firstChild(); item; item=item->nextSibling())
      {
          QString artist = item->text(Private::TRACK_ARTIST);
          if (!artist.isEmpty())
          {
              item->setText(Private::TRACK_ARTIST, QString::null);
              item->setText(Private::TRACK_TITLE, artist + Private::SEPARATOR + item->text(Private::TRACK_TITLE));
          }
      }
      d->ui->m_trackList->hideColumn(Private::TRACK_ARTIST);
      d->ui->m_trackList->adjustColumn(Private::TRACK_TITLE);
  }
}


  void CDInfoDialog::slotChangeEncoding()
  {
      kDebug() << k_funcinfo;

  KDialog* dialog = new KDialog(this);
  dialog->setCaption(i18n("Change Encoding"));
  dialog->setButtons( KDialog::Ok | KDialog::Cancel);
  dialog->setModal( true );


  QStringList songTitles;
  for (Q3ListViewItem *item = d->ui->m_trackList->firstChild(); item; item=item->nextSibling())
  {
      QString title = item->text(Private::TRACK_ARTIST).trimmed();
      if (!title.isEmpty())
          title.append(Private::SEPARATOR);
          title.append(item->text(Private::TRACK_TITLE).trimmed());
          songTitles << title;
      }

      KCDDB::CDInfoEncodingWidget* encWidget = new KCDDB::CDInfoEncodingWidget(
          dialog, d->ui->m_artist->text(),d->ui->m_title->text(), songTitles);

      dialog->setMainWidget(encWidget);

      if (dialog->exec())
      {
        KCharsets* charsets = KGlobal::charsets();
        QTextCodec* codec = charsets->codecForName(charsets->encodingForName(encWidget->selectedEncoding()));

        d->ui->m_artist->setText(codec->toUnicode(d->ui->m_artist->text().toLatin1()));
        d->ui->m_title->setText(codec->toUnicode(d->ui->m_title->text().toLatin1()));
        d->ui->m_genre->setItemText(d->ui->m_genre->currentIndex(), codec->toUnicode(d->ui->m_genre->currentText().toLatin1()));
        d->ui->m_comment->setText(codec->toUnicode(d->ui->m_comment->text().toLatin1()));

        for (Q3ListViewItem *item = d->ui->m_trackList->firstChild(); item; item=item->nextSibling())
        {
            item->setText(Private::TRACK_ARTIST,codec->toUnicode(item->text(Private::TRACK_ARTIST).toLatin1()));
            item->setText(Private::TRACK_TITLE,codec->toUnicode(item->text(Private::TRACK_TITLE).toLatin1()));
            item->setText(Private::TRACK_COMMENT,codec->toUnicode(item->text(Private::TRACK_COMMENT).toLatin1()));
        }
      }
  }
}

#include "cdinfodialog.moc"

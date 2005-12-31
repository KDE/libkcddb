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


#include <qtextcodec.h>
#include <kdebug.h>
#include <kdialogbase.h>
#include <qdatetime.h>
#include <kglobal.h>
#include <kcharsets.h>

#include "cdinfodialog.h"
#include "cdinfoencodingwidget.h"

using KCDDB::TrackInfo;

namespace KCDDB
{
  const char *CDInfoDialog::SEPARATOR = " / ";

  // FIXME Make a KDialog??
  CDInfoDialog::CDInfoDialog(QWidget* parent)
    : QWidget(parent), Ui::CDInfoDialogBase()
  {
      setupUi(this);
    
      m_categories = KCDDB::Categories();
      m_category->insertStringList(m_categories.i18nList());
      m_genres = KCDDB::Genres();
      m_genre->insertStringList(m_genres.i18nList());

      // We want control over the visibility of this column. See artistChanged().
      m_trackList->setColumnWidthMode(TRACK_ARTIST, Q3ListView::Manual);

      // Make the user-definable values in-place editable.
      m_trackList->setRenameable(TRACK_NUMBER, false);
      m_trackList->setRenameable(TRACK_TIME, false);
      m_trackList->setRenameable(TRACK_TITLE, true);
      m_trackList->setRenameable(TRACK_COMMENT, true);
      m_trackList->setRenameable(TRACK_ARTIST, true);
  }

  void CDInfoDialog::slotTrackSelected( Q3ListViewItem *item )
  {
      emit play(item->text(0).toUInt()-1);
  }

  void CDInfoDialog::slotNextTrack()
  {
      if (m_trackList->currentItem())
      {
          Q3ListViewItem *item = m_trackList->currentItem()->nextSibling();
          m_trackList->setSelected(item, true);
          m_trackList->ensureItemVisible(item);
      }
  }

  void CDInfoDialog::slotTrackDoubleClicked( Q3ListViewItem *item, const QPoint &, int column)
  {
      m_trackList->rename(item, column);
  }

  void CDInfoDialog::setInfo( const KCDDB::CDInfo &info, KCDDB::TrackOffsetList &trackStartFrames )
  {
      m_artist->setText(info.get(Artist).toString().stripWhiteSpace());
      m_title->setText(info.get(Title).toString().stripWhiteSpace());
      m_category->setCurrentText(m_categories.cddb2i18n(info.get(Category).toString()));

      // Make sure the revision is set before the genre to allow the genreChanged() handler to fire.
      m_revision->setText(QString::number(info.get("revision").toInt()));
      m_genre->setCurrentText(m_genres.cddb2i18n(info.get(Genre).toString()));
      m_year->setValue(info.get(Year).toInt());
      m_comment->setText(info.get(Comment).toString().stripWhiteSpace());
      m_id->setText(info.get("discid").toString().stripWhiteSpace());

      // Now do the individual tracks.
      unsigned tracks = info.numberOfTracks();
      m_length->setText(framesTime(trackStartFrames[tracks] - trackStartFrames[0]));
      m_trackList->clear();
      for (unsigned i = 0; i < tracks; i++)
      {
          Q3ListViewItem *item = new Q3ListViewItem(m_trackList, 0);

          TrackInfo ti(info.track(i));

          item->setText(TRACK_NUMBER, QString().sprintf("%02d", i + 1));
          item->setText(TRACK_TIME, framesTime(trackStartFrames[i + 1] - trackStartFrames[i]));
          item->setText(TRACK_ARTIST, ti.get(Artist).toString());
          item->setText(TRACK_TITLE, ti.get(Title).toString());
          item->setText(TRACK_COMMENT, ti.get(Comment).toString());
      }
      // FIXME KDE4: handle playorder here too, once KCDDBInfo::CDInfo is updated.

      if (info.get(Artist).toString() == "Various" || m_multiple->isChecked()){
          m_trackList->adjustColumn(TRACK_ARTIST);
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
      KCDDB::CDInfo info;

      info.set(Artist, m_artist->text().stripWhiteSpace());
      info.set(Title, m_title->text().stripWhiteSpace());
      info.set(Category, m_categories.i18n2cddb(m_category->currentText()));
      info.set(Genre, m_genres.i18n2cddb(m_genre->currentText()));
      info.set(Year, m_year->value());
      info.set(Comment, m_comment->text().stripWhiteSpace());
      info.set("revision", m_revision->text().stripWhiteSpace().toUInt());
      info.set("discid", m_id->text().stripWhiteSpace());
      int i=0;
      for (Q3ListViewItem *item = m_trackList->firstChild(); item; item=item->nextSibling())
      {
          TrackInfo& track = info.track(i);
          track.set(Artist,item->text(TRACK_ARTIST).stripWhiteSpace());
          track.set(Title,item->text(TRACK_TITLE).stripWhiteSpace());
          track.set(Comment,item->text(TRACK_COMMENT).stripWhiteSpace());
          i++;
          // FIXME KDE4: handle track lengths here too, once KCDDBInfo::CDInfo is updated.
      }
      // FIXME KDE4: handle playorder here too, once KCDDBInfo::CDInfo is updated.
      return info;
  }


  void CDInfoDialog::artistChanged( const QString &newArtist )
  {
      // Enable special handling of compilations.
      if (newArtist.stripWhiteSpace().compare("Various")) {
          m_multiple->setChecked(false);
      } else {
          m_multiple->setChecked(true);
      }
  }

  void CDInfoDialog::genreChanged( const QString &newGenre )
  {
      // Disable changes to category if the version number indicates that a record
      // is already in the database, or if the genre is poorly set. The latter
      // condition also provides a "back-door" override.
      m_category->setEnabled((m_revision->text().stripWhiteSpace().toUInt() < 1) ||
                              (newGenre.compare("Unknown") == 0));
  }


  void CDInfoDialog::slotMultipleArtists( bool hasMultipleArtist)
  {
      if(hasMultipleArtist){
          for (Q3ListViewItem *item = m_trackList->firstChild(); item; item=item->nextSibling())
          {
              QString title = item->text(TRACK_TITLE);
              int separator = title.find(SEPARATOR);
              if (separator != -1)
              {
                  // Artists probably entered already
                  item->setText(TRACK_ARTIST, title.left(separator));
                  item->setText(TRACK_TITLE, title.mid(separator + 3));
              }
          }
          m_trackList->adjustColumn(TRACK_ARTIST);
          m_trackList->adjustColumn(TRACK_TITLE);
      } 
      else{
          for (Q3ListViewItem *item = m_trackList->firstChild(); item; item=item->nextSibling())
          {
              QString artist = item->text(TRACK_ARTIST);
              if (!artist.isEmpty())
              {
                  item->setText(TRACK_ARTIST, QString::null);
                  item->setText(TRACK_TITLE, artist + SEPARATOR + item->text(TRACK_TITLE));
              }
          }
          m_trackList->hideColumn(TRACK_ARTIST);
          m_trackList->adjustColumn(TRACK_TITLE);
      }
  }


  void CDInfoDialog::slotChangeEncoding()
  {
      kdDebug() << k_funcinfo << endl;
      
      KDialogBase* dialog = new KDialogBase(this, 0, true, i18n("Change Encoding"),
          KDialogBase::Ok | KDialogBase::Cancel);

      QStringList songTitles;
      for (Q3ListViewItem *item = m_trackList->firstChild(); item; item=item->nextSibling())
      {
          QString title = item->text(TRACK_ARTIST).stripWhiteSpace();
          if (!title.isEmpty())
              title.append(SEPARATOR);
          title.append(item->text(TRACK_TITLE).stripWhiteSpace());
          songTitles << title;
      }

      KCDDB::CDInfoEncodingWidget* encWidget = new KCDDB::CDInfoEncodingWidget(
          dialog, m_artist->text(),m_title->text(), songTitles);

      dialog->setMainWidget(encWidget);

      if (dialog->exec())
      {
        KCharsets* charsets = KGlobal::charsets();
        QTextCodec* codec = charsets->codecForName(charsets->encodingForName(encWidget->selectedEncoding()));

        m_artist->setText(codec->toUnicode(m_artist->text().latin1()));
        m_title->setText(codec->toUnicode(m_title->text().latin1()));
        m_genre->setCurrentText(codec->toUnicode(m_genre->currentText().latin1()));
        m_comment->setText(codec->toUnicode(m_comment->text().latin1()));

        for (Q3ListViewItem *item = m_trackList->firstChild(); item; item=item->nextSibling())
        {
            item->setText(TRACK_ARTIST,codec->toUnicode(item->text(TRACK_ARTIST).latin1()));
            item->setText(TRACK_TITLE,codec->toUnicode(item->text(TRACK_TITLE).latin1()));
            item->setText(TRACK_COMMENT,codec->toUnicode(item->text(TRACK_COMMENT).latin1()));
        }
      }
  }
}

#include "cdinfodialog.moc"

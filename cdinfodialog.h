/*
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

#ifndef KCDDB_CDINFODIALOG_H
#define KCDDB_CDINFODIALOG_H

#include "cdinfodialogbase.h"

namespace KCDDB
{
    class CDInfoDialog : public QWidget, public Ui::CDInfoDialogBase
    {
    Q_OBJECT
    public:
        CDInfoDialog(QWidget* parent);
        virtual ~CDInfoDialog() { }

        void setInfo( const KCDDB::CDInfo &info, KCDDB::TrackOffsetList &trackStartFrames );
        KCDDB::CDInfo info() const;

    signals:
        void play( int );

    private slots:
        void slotTrackSelected( Q3ListViewItem* );
        void slotNextTrack();
        void slotTrackDoubleClicked( Q3ListViewItem *item, const QPoint &, int column );
        void artistChanged( const QString &newArtist );
        void genreChanged( const QString &newGenre );
        void slotMultipleArtists( bool hasMultipleArtist );
        void slotChangeEncoding();

    private:
        QString framesTime(unsigned frames);

        KCDDB::Genres m_genres;
        KCDDB::Categories m_categories;
        static const char *SEPARATOR;
        static const unsigned TRACK_TIME = 1;
        static const unsigned TRACK_NUMBER = 0;
        static const unsigned TRACK_TITLE = 2;
        static const unsigned TRACK_COMMENT = 3;
        static const unsigned TRACK_ARTIST = 4;
    } ;
}

#endif // KCDDB_CDINFODIALOG_H

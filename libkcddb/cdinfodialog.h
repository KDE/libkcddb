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

#include <libkcddb/kcddb_export.h>
#include <libkcddb/cdinfo.h>
#include <libkcddb/kcddb.h>

#include <KDE/KDialog>

class QStandardItemModel;
class QModelIndex;

namespace KCDDB
{
    class KCDDB_EXPORT CDInfoDialog : public KDialog
    {
    Q_OBJECT
    public:
        explicit CDInfoDialog(QWidget* parent);
        virtual ~CDInfoDialog();

        void setInfo( const KCDDB::CDInfo &info, const KCDDB::TrackOffsetList &trackStartFrames );
        KCDDB::CDInfo info() const;

    Q_SIGNALS:
        void play( int );

    private Q_SLOTS:
        void slotTrackSelected( const QModelIndex &index );
        void slotNextTrack();
        void slotTrackDoubleClicked(const QModelIndex &index);
        void artistChanged( const QString &newArtist );
        void genreChanged( const QString &newGenre );
        void slotMultipleArtists( bool hasMultipleArtist );
        void slotChangeEncoding();

    private:
        QString framesTime(unsigned frames);
	QStandardItemModel *m_trackModel;

        class Private;
        Private * const d;
    } ;
}

#endif // KCDDB_CDINFODIALOG_H

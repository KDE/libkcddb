/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002 Benjamin Meyer <ben-devel@meyerhome.net>
  CopyRight (C) 2002 Nadeem Hasan <nhasan@kde.org>

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

#include <QRegExp>
#include <qstringlist.h>

#include <kdebug.h>
#include <kstringhandler.h>
#include <klocale.h>

#include "cddb.h"
#include "categories.h"

namespace KCDDB
{
  CDDB::CDDB()
    : user_( "libkcddb-user" ),
      localHostName_( "localHost" ),
      readOnly_( false )
  {

  }

  CDDB::~CDDB()
  {
    // Empty.
  }

    QString
  CDDB::trackOffsetListToId()
  {
    return trackOffsetListToId( trackOffsetList_ );
  }
    QString
  CDDB::trackOffsetListToId( const TrackOffsetList & list )
  {
    // Taken from version by Michael Matz in kio_audiocd.
    unsigned int id = 0;
    int numTracks = list.count() - 1;

    // The last two in the list are disc begin and disc end.
    for ( int i = numTracks-1; i >= 0; i-- )
    {
      int n = list[ i ]/75;
      while ( n > 0 )
      {
        id += n % 10;
        n /= 10;
      }
    }

    unsigned int l = list[numTracks] / 75;
    l -= list[0] / 75;

    id = ( ( id % 255 ) << 24 ) | ( l << 8 ) | numTracks;

    return QString::number( id, 16 ).rightJustified( 8, '0' );
  }

    QString
  CDDB::trackOffsetListToString()
  {
    QString ret;
    uint numTracks = trackOffsetList_.count()-1;

    // Disc start.
    ret.append( QString::number( numTracks ) );
    ret.append( " " );

    for ( uint i = 0; i < numTracks; i++ )
    {
      ret.append( QString::number( trackOffsetList_[ i ] ) );
      ret.append( " " );
    }

    unsigned int discLengthInSec = ( trackOffsetList_[ numTracks ] ) / 75;

    ret.append( QString::number( discLengthInSec ) );

    return ret;
  }

    uint
  CDDB::statusCode( const QString & line )
  {
    QStringList tokenList = line.split(' ', QString::SkipEmptyParts );

    uint serverStatus = tokenList[ 0 ].toUInt();

    return serverStatus;
  }

    QStringList
  CDDB::cacheFilenames(const TrackOffsetList& offsetList)
  {
    QStringList filenames;
    Categories c;
    QStringList categories = c.cddbList();
    foreach(QString dir, categories)
      filenames << dir+"/"+trackOffsetListToId(offsetList);

    return filenames;
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

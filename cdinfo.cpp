/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002 Benjamin Meyer <ben-devel@meyerhome.net>

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
  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA 02111-1307, USA.
*/

#include <kdebug.h>
#include <kstringhandler.h>

#include "cdinfo.h"
#include "cddb.h"

namespace KCDDB
{
  TrackInfo::TrackInfo()
  {
  }

  TrackInfo::~TrackInfo()
  {
  }

  CDInfo::CDInfo()
    : year(0),
      length(0),
      revision(0)
  {
  }

  CDInfo::~CDInfo()
  {
  }

    bool
  CDInfo::load(const QString & s)
  {
    return load(QStringList::split('\n', s));
  }

    bool
  CDInfo::load(const QStringList & lineList)
  {
    clear();

    // We'll append to this until we've seen all the lines, then parse it after.
    QString dtitle;

    QStringList::ConstIterator it = lineList.begin();

    QRegExp rev("# Revision: (\\d+)");

    while ( it != lineList.end() )
    {
      QString line(*it);
      ++it;

      QStringList tokenList = KStringHandler::perlSplit('=', line, 2);

      if (rev.search(line) != -1)
      {
        revision = rev.cap(1).toUInt();;
	continue;
      }

      if (2 != tokenList.count())
        continue;

      QString key   = tokenList[0].stripWhiteSpace();
      QString value = tokenList[1].stripWhiteSpace();

      value = unescape ( value );

      if ( "DISCID" == key )
      {
        id = value;
      }
      else if ( "DTITLE" == key )
      {
        dtitle += value;
      }
      else if ( "DYEAR" == key )
      {
        year = value.toUInt();
      }
      else if ( "DGENRE" == key )
      {
        genre = value;
      }
      else if ( "TTITLE" == key.left( 6 ) )
      {
        uint trackNumber = key.mid(6).toUInt();

        checkTrack( trackNumber );

        trackInfoList[ trackNumber ].title.append( value );
      }
      else if ( "EXTD" == key )
      {
        extd.append( value );
      }
      else if ( "EXTT" == key.left( 4 ) )
      {
        uint trackNumber = key.mid( 4 ).toUInt();

        checkTrack( trackNumber );

        trackInfoList[ trackNumber ].extt.append( value );
      }
    }

    int slashPos = dtitle.find('/');

    if (-1 == slashPos)
    {
      // Use string for title _and_ artist.
      artist = title = dtitle;
    }
    else
    {
      artist  = dtitle.left(slashPos).stripWhiteSpace();
      title   = dtitle.mid(slashPos + 1).stripWhiteSpace();
    }

    if ( genre.isEmpty() )
      genre = "Unknown";

    kdDebug(60010) << "Loaded CDInfo for " << id << endl;

    return true;
  }

    QString
  CDInfo::toString(bool submit) const
  {
    QString s;

    if (revision != 0)
      s += "# Revision: " + QString::number(revision) + "\n\n";

    if (submit)
    {
      s += "#\n";
      s += QString("# Submitted via: %1 %2\n").arg(CDDB::clientName(),
        CDDB::clientVersion());
    }

    s += "DISCID=" + escape( id ) + "\n";
    s += "DTITLE=" + escape( artist ) + " / " + escape( title ) + "\n";
    s += "DYEAR=" + (0 == year ? QString::null : QString::number(year)) + "\n";
    s += "DGENRE=" + escape( genre ) + "\n";

    for (uint i = 0; i < trackInfoList.count(); ++i)
    {
      s += QString( "TTITLE%1=%2\n" )
                .arg( i )
                .arg( escape( trackInfoList[ i ].title ) );
    }

    s += "EXTD=" + escape( extd ) + "\n";

    for (uint i = 0; i < trackInfoList.count(); ++i)
    {
      s += QString( "EXTT%1=%2\n" )
                .arg( i ) 
                .arg( trackInfoList[ i ].extt );
    }

    s +="PLAYORDER=\n";

    return s;
  }

    void
  CDInfo::checkTrack( uint trackNumber )
  {
    if ( trackInfoList.count() < trackNumber + 1 )
    {
      while ( trackInfoList.count() < trackNumber + 1 )
        trackInfoList.append(TrackInfo());
    }
  }

    QString
  CDInfo::escape( const QString &value ) const
  {
    QString s = value;
    s.replace( QRegExp( "\n" ), "\\n" );
    s.replace( QRegExp( "\t" ), "\\t" );
    s.replace( QRegExp( "\\" ), "\\\\" );

    return s;
  }

    QString
  CDInfo::unescape( const QString &value ) const
  {
    QString s = value;
    s.replace( QRegExp( "\\n" ), "\n" );
    s.replace( QRegExp( "\\t" ), "\t" );
    s.replace( QRegExp( "\\\\" ), "\\" );

    return s;
  }

    void
  CDInfo::clear()
  {
    id = artist = title = genre = extd = QString::null;
    length = year = revision = 0;
    trackInfoList.clear();
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

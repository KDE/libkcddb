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
  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA 02111-1307, USA.
*/

#include <qregexp.h>

#include <kdebug.h>
#include <kstringhandler.h>

#include "lookup.h"

namespace KCDDB
{
  Lookup::Lookup()
          : readOnly_( false )
  {
    // Empty.
  }

  Lookup::~Lookup()
  {
    // Empty.
  }

    QString
  Lookup::trackOffsetListToId( const TrackOffsetList & list )
  {
    if ( list.count() < 3 )
      return QString::null;

    // Taken from version by Michael Matz in kio_audiocd.
    unsigned int id = 0;
    int trackCount = list.count() - 2;

    // The last two in the list are disc begin and disc end.
    for ( int i = trackCount-1; i >= 0; i-- )
    {
      int n = list[ i ]/75;
      while ( n > 0 )
      {
        id += n % 10;
        n /= 10;
      }
    }

    unsigned int l = list[ trackCount + 1 ];

    l -= list[ trackCount ];
    l /= 75;

    id = ( ( id % 255 ) << 24 ) | ( l << 8 ) | trackCount;

    return QString::number( id, 16 ).rightJustify( 8, '0' );
  }

    QString
  Lookup::trackOffsetListToString( const TrackOffsetList & list )
  {
    if (  list.count() < 3 )
    {
      kdDebug() << "Bogus list. Less than 3 entries." << endl;
      return QString::null;
    }

    QString ret;

    // Disc start.
    ret.append(  QString::number(  list[  list.count()-2 ] ) );
    ret.append(  " " );

    for (  uint i = 0; i < list.count() - 2; i++ )
    {
      ret.append(  QString::number(  list[  i ] ) );
      ret.append(  " " );
    }

    unsigned int discLengthInSeconds = (  list[  list.count() - 1 ] ) / 75;

    // Disc length in seconds.
    ret.append(  QString::number(  discLengthInSeconds ) );

    return ret;
  }

    bool
  Lookup::parseGreeting( const QString & line )
  {
    QStringList tokenList = QStringList::split( ' ', line );

    uint serverStatus = tokenList[ 0 ].toUInt();

    if ( 200 == serverStatus )
    {
      kdDebug() << "Server response: read-only" << endl;
      readOnly_ = true;
    }
    else if ( 201 == serverStatus )
    {
      kdDebug() << "Server response: read-write" << endl;
    }
    else
    {
      kdDebug() << "Server response: bugger off" << endl;
      return false;
    }

    return true;
  }

    bool
  Lookup::parseHandshake( const QString & line )
  {
    QStringList tokenList = QStringList::split( ' ', line );

    uint serverStatus = tokenList[ 0 ].toUInt();

    if ( ( 200 != serverStatus ) && ( 402 != serverStatus ) )
    {
      kdDebug() << "Handshake was too tight. Letting go." << endl;
      return false;
    }

    kdDebug() << "Handshake was warm and firm" << endl;

    return true;
  }

     bool
  Lookup::parseQuery( const QString & line, uint *status )
  {
    QStringList tokenList = QStringList::split( ' ', line );

    uint serverStatus  = tokenList[ 0 ].toUInt();
    *status = serverStatus;

    if ( 200 == serverStatus )
    {
      QStringList tokenList = QStringList::split( ' ', line );
      matchList_.append( qMakePair( tokenList[ 0 ], tokenList[ 1 ] ) );
      return true;
    }
    else if ( ( 211 == serverStatus ) || ( 210 == serverStatus ) )
    {
      kdDebug() << "Server found multiple matches" << endl;
      return true;
    }

    return false;
  }

    void
  Lookup::parseExtraMatch( const QString & line )
  {
    QStringList tokenList = QStringList::split( ' ', line );
    matchList_.append( qMakePair( tokenList[ 0 ], tokenList[ 1 ] ) );
  }

    CDInfo
  Lookup::parseCDInfo( const QStringList & lineList )
  {
    CDInfo info;

    QStringList::ConstIterator it;

    for ( it = lineList.begin(); it != lineList.end(); ++it )
    {
      QString line( *it );

      QStringList tokenList = KStringHandler::perlSplit( '=', line, 2 );

      if ( 2 != tokenList.count() )
        continue;

      QString key   = tokenList[ 0 ];
      QString value = tokenList[ 1 ];

      value.replace( QRegExp( "\\n" ), "\n" );
      value.replace( QRegExp( "\\t" ), "\t" );
      value.replace( QRegExp( "\\\\" ), "\\");

      if ( "DTITLE" == key )
      {
        int slashPos = value.find( '/' );

        if ( -1 == slashPos )
        {
          // Use string for title _and_ artist.
          info.artist = info.title = value.stripWhiteSpace();
        }
        else
        {
          info.artist  = value.left( slashPos ).stripWhiteSpace();
          info.title   = value.mid( slashPos + 1 ).stripWhiteSpace();
        }
      }
      else if ( "DYEAR" == key )
      {
        info.year = value.toUInt();
      }
      else if ( "DGENRE" == key )
      {
        info.genre = value.stripWhiteSpace();
      }
      else if ( "TTITLE" == key.left( 6 ) )
      {
        uint trackNumber = key.mid( 6 ).toUInt();

        TrackInfo trackInfo;
        trackInfo.title = value.stripWhiteSpace();

        while ( info.trackInfoList.size() < trackNumber + 1 )
        {
          info.trackInfoList.append( TrackInfo() );
        }

        info.trackInfoList[ trackNumber ] = trackInfo;
      }
    }

    return info;
  }

    QString
  Lookup::resultToString(Result r)
  {
    switch (r)
    {
      case Success:
        return "Success";
        break;

      case HostNotFound:
        return "HostNotFound";
        break;

      case NoResponse:
        return "NoResponse";
        break;

      case NoRecordFound:
        return "NoSuchCD";
        break;

      case CannotSave:
        return "CannotSave";
        break;

      default:
        return "UnknownError";
        break;
    }
  }

    QString
  Lookup::readLine()
  {
    if ( KExtendedSocket::connected != socket_.socketStatus() )
    {
      kdDebug() << "socket status: " << socket_.socketStatus() << endl;
      return QString::null;
    }

    QCString buf;

    int c = socket_.getch();

    while ( '\n' != c )
    {
      buf += c;
      c = socket_.getch();
    }

    kdDebug() << "READ: [" << buf << "]" << endl;
    return QString::fromLatin1( buf.data(), buf.length() );
  }

    void
  Lookup::writeLine( const QString & line )
  {
    if ( KExtendedSocket::connected != socket_.socketStatus() )
    {
      kdDebug() << "socket status: " << socket_.socketStatus() << endl;
      return;
    }

    kdDebug() << "WRITE: [" << line << "]" << endl;
    QCString buf = line.latin1();
    buf.append( "\n" );

    socket_.writeBlock( buf.data(), buf.length() );
  }

    Lookup::Transport
  Lookup::stringToTransport(const QString & s)
  {
    if ("HTTP" == s)
      return HTTP;
    else
      return CDDB;
  }

    QString
  Lookup::transportToString(uint t)
  {
    switch (Transport(t))
    {
      case HTTP:
        return "HTTP";
        break;

      default:
        return "CDDB";
        break;
    }
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

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
#include <qstringlist.h>

#include <kdebug.h>
#include <kstringhandler.h>
#include <klocale.h>

#include "cddb.h"

namespace KCDDB
{
  CDDB::CDDB()
    : user_( "libkcddb-user" ),
      localHostName_( "localHost" ),
      readOnly_( false )
  {
    socket_.setTimeout( 60 );
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
    int numTracks = list.count() - 2;

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

    unsigned int l = (list[numTracks + 1] + 1) / 75;
    l -= list[0] / 75;

    id = ( ( id % 255 ) << 24 ) | ( l << 8 ) | numTracks;

    return QString::number( id, 16 ).rightJustify( 8, '0' );
  }

    QString
  CDDB::trackOffsetListToString()
  {
    QString ret;
    uint numTracks = trackOffsetList_.count()-2;

    // Disc start.
    ret.append( QString::number( numTracks ) );
    ret.append( " " );

    for ( uint i = 0; i < numTracks; i++ )
    {
      ret.append( QString::number( trackOffsetList_[ i ] ) );
      ret.append( " " );
    }

    unsigned int discLengthInSec = ( trackOffsetList_[ numTracks+1 ] ) / 75;

    // Disc length in seconds.
    ret.append( QString::number( discLengthInSec ) );

    return ret;
  }

    bool
  CDDB::parseGreeting( const QString & line )
  {
    uint serverStatus = statusCode( line );

    if ( 200 == serverStatus )
    {
      kdDebug(60010) << "Server response: read-only" << endl;
      readOnly_ = true;
    }
    else if ( 201 == serverStatus )
    {
      kdDebug(60010) << "Server response: read-write" << endl;
    }
    else
    {
      kdDebug(60010) << "Server response: bugger off" << endl;
      return false;
    }

    return true;
  }

    bool
  CDDB::parseHandshake( const QString & line )
  {
    uint serverStatus = statusCode( line );

    if ( ( 200 != serverStatus ) && ( 402 != serverStatus ) )
    {
      kdDebug(60010) << "Handshake was too tight. Letting go." << endl;
      return false;
    }

    kdDebug(60010) << "Handshake was warm and firm" << endl;

    return true;
  }

    QString
  CDDB::readLine()
  {
    if ( !isConnected() )
    {
      kdDebug(60010) << "socket status: " << socket_.socketStatus() << endl;
      return QString::null;
    }

    const uint maxRead = 4096;
    QByteArray buf( maxRead );

    if (socket_.readLine( buf.data(), maxRead - 1 ) == -1)
    {
        // error!
        buf[0] = '\0';
    }

    return QString( buf );
  }

    Q_LONG
  CDDB::writeLine( const QString & line )
  {
    if ( !isConnected() )
    {
      kdDebug(60010) << "socket status: " << socket_.socketStatus() << endl;
      return -1;
    }

    kdDebug(60010) << "WRITE: [" << line << "]" << endl;
    QCString buf = line.latin1();
    buf.append( "\n" );

    return socket_.writeBlock( buf.data(), buf.length() );
  }

    uint
  CDDB::statusCode( const QString & line )
  {
    QStringList tokenList = QStringList::split( ' ', line );

    uint serverStatus = tokenList[ 0 ].toUInt();

    return serverStatus;
  }

/*    CDDB::Transport
  CDDB::stringToTransport(const QString & s)
  {
    if  ("HTTP" == s )
      return HTTP;
    else if ( "CDDBP" == s )
      return CDDBP;
    else
      return SMTP;
  }*/

    QString
  CDDB::resultToString(Result r)
  {
    switch (r)
    {
      case Success:
        return i18n("Success");
        break;

/*  Add when permitted to add strings or for 3.4/4.0 whichever comes first
      case ServerError:
        return i18n("Server error");
        break;
*/

      case HostNotFound:
        return i18n("Host not found");
        break;

      case NoResponse:
        return i18n("No response");
        break;

      case NoRecordFound:
        return i18n("No record found");
        break;

      case MultipleRecordFound:
        return i18n("Multiple records found");
        break;

      case CannotSave:
        return i18n("Cannot save");
        break;

      case InvalidCategory:
        return i18n("Invalid category");
        break;

      default:
        return i18n("Unknown error");
        break;
    }
  }

/*    QString
  CDDB::transportToString(uint t)
  {
    switch (Transport(t))
    {
      case HTTP:
        return "HTTP";
        break;

      case CDDBP:
        return "CDDBP";
        break;

      case SMTP:
        return "SMTP";
        break;

      default:
        return "UnknownTransport";
        break;
    }
  }*/
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

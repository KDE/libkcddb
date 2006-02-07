/*
  Copyright ( C ) 2002 Rik Hemsley (  rikkus ) <rik@kde.org>
  Copyright ( C ) 2002 Benjamin Meyer <ben-devel@meyerhome.net>
  Copyright ( C ) 2002 Nadeem Hasan <nhasan@kde.org>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (  at your option ) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#include <kdebug.h>

#include "cddbplookup.h"
#include <qbytearray.h>

namespace KCDDB
{
  CDDBPLookup::CDDBPLookup()
    : Lookup()
  {

  }

  CDDBPLookup::~CDDBPLookup()
  {
    delete socket_;
  }

    void
  CDDBPLookup::sendHandshake()
  {
    QString handshake = QString( "cddb hello %1 %2 %3 %4" )
        .arg( user_ )
        .arg( localHostName_ )
        .arg( clientName() )
        .arg( clientVersion() );

    writeLine( handshake );
  }

    void
  CDDBPLookup::sendProto()
  {
    writeLine( "proto 6" );
  }

    void
  CDDBPLookup::sendQuery()
  {
    QString query = QString( "cddb query %1 %2" )
        .arg( trackOffsetListToId() )
        .arg( trackOffsetListToString() );

    writeLine( query );
  }

    void
  CDDBPLookup::sendRead( const CDDBMatch & match )
  {
    category_  = match.first;
    QString discid    = match.second;

    QString readRequest = QString( "cddb read %1 %2" )
        .arg( category_ )
        .arg( discid );

    writeLine( readRequest );
  }

    void
  CDDBPLookup::sendQuit()
  {
    writeLine( "quit" );
  }

    void
  CDDBPLookup::close()
  {
    kDebug(60010) << "Disconnect from server..." << endl;
    if ( isConnected() )
    {
      socket_->close();
    }
  }

    Q_LONG
  CDDBPLookup::writeLine( const QString & line )
  {
    if ( !isConnected() )
    {
      kDebug(60010) << "socket status: " << socket_->state() << endl;
      return -1;
    }

    kDebug(60010) << "WRITE: [" << line << "]" << endl;
    QByteArray buf(line.utf8());
    buf.append( '\n' );

    return socket_->writeBlock( buf.data(), buf.length() );
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

/*
  Copyright ( C ) 2002 Rik Hemsley (  rikkus ) <rik@kde.org>
  Copyright ( C ) 2002 Benjamin Meyer <ben-devel@meyerhome.net>
  Copyright ( C ) 2002 Nadeem Hasan <nhasan@kde.org>
  Copyright ( C ) 2016 Angelo Scarnà <angelo.scarna@codelinsoft.it>

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

#include "cddbplookup.h"

#include <QDebug>

#include <qbytearray.h>

namespace KCDDB
{
  CDDBPLookup::CDDBPLookup()
    : Lookup()
    , socket_(0)
  {

  }

  CDDBPLookup::~CDDBPLookup()
  {
    if (socket_)
      delete socket_;
  }

    void
  CDDBPLookup::sendHandshake()
  {
      QString handshake = QString::fromLatin1( "cddb hello %1 %2 %3 %4" )
        .arg( user_ )
        .arg( localHostName_ )
        .arg( clientName() )
        .arg( clientVersion() );

    writeLine( handshake );
  }

    void
  CDDBPLookup::sendProto()
  {
    writeLine( QLatin1String( "proto 6" ) );
  }

    void
  CDDBPLookup::sendQuery()
  {
      QString query = QString::fromLatin1( "cddb query %1 %2" )
        .arg( trackOffsetListToId() )
        .arg( trackOffsetListToString() );

    writeLine( query );
  }

    void
  CDDBPLookup::sendRead( const CDDBMatch & match )
  {
    category_  = match.first;
    discid_    = match.second;

    QString readRequest = QString::fromLatin1( "cddb read %1 %2" )
        .arg( category_ )
        .arg( discid_ );

    writeLine( readRequest );
  }

    void
  CDDBPLookup::sendQuit()
  {
    writeLine( QLatin1String( "quit" ) );
  }

    void
  CDDBPLookup::close()
  {
    qDebug() << "Disconnect from server...";
    if ( isConnected() )
    {
      socket_->close();
    }
  }

    bool
  CDDBPLookup::parseGreeting( const QString & line )
  {
    uint serverStatus = statusCode( line );

    if ( 200 == serverStatus )
    {
      qDebug() << "Server response: read-only";
      readOnly_ = true;
    }
    else if ( 201 == serverStatus )
    {
      qDebug() << "Server response: read-write";
    }
    else
    {
      qDebug() << "Server response: bugger off";
      return false;
    }

    return true;
  }

    bool
  CDDBPLookup::parseHandshake( const QString & line )
  {
    uint serverStatus = statusCode( line );

    if ( ( 200 != serverStatus ) && ( 402 != serverStatus ) )
    {
      qDebug() << "Handshake was too tight. Letting go.";
      return false;
    }

    qDebug() << "Handshake was warm and firm";

    return true;
  }


    qint64
  CDDBPLookup::writeLine( const QString & line )
  {
    if ( !isConnected() )
    {
      qDebug() << "socket status: " << socket_->state();
      return -1;
    }

    qDebug() << "WRITE: [" << line << "]";
    QByteArray buf(line.toUtf8());
    buf.append( '\n' );

    return socket_->write( buf );
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

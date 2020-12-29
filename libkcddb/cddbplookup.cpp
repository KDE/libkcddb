/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (  rikkus ) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>
    SPDX-FileCopyrightText: 2002 Nadeem Hasan <nhasan@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "cddbplookup.h"
#include "logging.h"

#include <QByteArray>

namespace KCDDB
{
  CDDBPLookup::CDDBPLookup()
    : Lookup()
    , socket_(nullptr)
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
	qCDebug(LIBKCDDB) << "Disconnect from server...";
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
	  qCDebug(LIBKCDDB) << "Server response: read-only";
      readOnly_ = true;
    }
    else if ( 201 == serverStatus )
    {
	  qCDebug(LIBKCDDB) << "Server response: read-write";
    }
    else
    {
	  qCDebug(LIBKCDDB) << "Server response: bugger off";
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
	  qCDebug(LIBKCDDB) << "Handshake was too tight. Letting go.";
      return false;
    }

	qCDebug(LIBKCDDB) << "Handshake was warm and firm";

    return true;
  }


    qint64
  CDDBPLookup::writeLine( const QString & line )
  {
    if ( !isConnected() )
    {
	  qCDebug(LIBKCDDB) << "socket status: " << socket_->state();
      return -1;
    }

	qCDebug(LIBKCDDB) << "WRITE: [" << line << "]";
    QByteArray buf(line.toUtf8());
    buf.append( '\n' );

    return socket_->write( buf );
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

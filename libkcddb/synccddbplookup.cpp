/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002 Benjamin Meyer <ben-devel@meyerhome.net>
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

#include "synccddbplookup.h"
#include "logging.h"

#include <qstringlist.h>
#include <ksocketfactory.h>

namespace KCDDB
{
  SyncCDDBPLookup::SyncCDDBPLookup()
    : CDDBPLookup()
  {
  }

  SyncCDDBPLookup::~SyncCDDBPLookup()
  {
    // Empty.
  }

    Result
  SyncCDDBPLookup::lookup
  (
    const QString         & hostName,
    uint                    port,
    const TrackOffsetList & trackOffsetList
  )
  {
    trackOffsetList_ = trackOffsetList;

    socket_ = KSocketFactory::synchronousConnectToHost(QLatin1String( "cddbp" ), hostName, port);

    if ( !socket_->isValid() )
    {
	  qCDebug(LIBKCDDB) << "Couldn't connect to " << socket_->peerName() << ":" << socket_->peerPort();
	  qCDebug(LIBKCDDB) << "Socket error: " << socket_->errorString();

      if ( socket_->error() == QAbstractSocket::HostNotFoundError )
        return HostNotFound;
      else if ( socket_->error() == QAbstractSocket::SocketTimeoutError )
        return NoResponse;
      else
        return UnknownError;
    }

    Result result;

    // Try a handshake.
    result = shakeHands();
    if ( Success != result )
      return result;

    // Run a query.
    result = runQuery();
    if ( Success != result )
      return result;

    if (matchList_.isEmpty())
      return NoRecordFound;

	qCDebug(LIBKCDDB) << matchList_.count() << " matches found.";

    // For each match, read the cd info from the server and save it to
    // cdInfoList.
    CDDBMatchList::ConstIterator matchIt = matchList_.constBegin();

    while ( matchIt != matchList_.constEnd() )
    {
      CDDBMatch match( *matchIt );
      result = matchToCDInfo( match );
      ++matchIt;
    }

    sendQuit();

    close();

    return Success;
  }

    Result
  SyncCDDBPLookup::shakeHands()
  {
    QString line = readLine();

    if ( !parseGreeting( line ) )
      return ServerError;

    sendHandshake();

    line = readLine();

    if ( !parseHandshake( line ) )
      return ServerError;

    sendProto();

    // Ignore the response for now
    readLine();

    return Success;
  }

    Result
  SyncCDDBPLookup::runQuery()
  {
    Result result;

    sendQuery();

    QString line = readLine();
    result = parseQuery( line );

    if ( ServerError == result )
      return ServerError;

    if ( MultipleRecordFound == result )
    {
      // We have multiple matches
      line = readLine();

      while (!line.startsWith(QLatin1String( "." )) && !line.isNull() )
      {
        parseExtraMatch( line );
        line = readLine();
      }
    }

    return Success;
  }

    Result
  SyncCDDBPLookup::matchToCDInfo( const CDDBMatch & match )
  {
    sendRead( match );

    QString line = readLine();

    Result result = parseRead( line );
    if ( Success != result )
      return result;

    QStringList lineList;
    line = readLine();

    while ( !line.startsWith(QLatin1String( "." )) && !line.isNull() )
    {
      lineList.append( line );
      line = readLine();
    }

    CDInfo info;

    if ( info.load( lineList ) )
    {
      info.set( QLatin1String( "category" ), category_ );
      info.set( QLatin1String( "discid" ), discid_ );
      info.set( QLatin1String( "source" ), QLatin1String( "freedb" ) );
      cdInfoList_.append( info );
    }

    return Success;
  }

    QString
  SyncCDDBPLookup::readLine()
  {
    if ( !isConnected() )
    {
	  qCDebug(LIBKCDDB) << "socket status: " << socket_->state();
      return QString();
    }

    if (!socket_->canReadLine())
    {
      if (!socket_->waitForReadyRead(-1))
        return QString();
    }

    return QString::fromUtf8(socket_->readLine());
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

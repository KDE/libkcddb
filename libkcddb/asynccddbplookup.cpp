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

#include "asynccddbplookup.h"

#include <kdebug.h>
#include <ksocketfactory.h>

namespace KCDDB
{
  AsyncCDDBPLookup::AsyncCDDBPLookup()
    : CDDBPLookup(),
      state_(Idle)
  {

  }

  AsyncCDDBPLookup::~AsyncCDDBPLookup()
  {
  }

    Result
  AsyncCDDBPLookup::lookup
  (
    const QString         & hostname,
    uint                    port,
    const TrackOffsetList & trackOffsetList
  )
  {
    socket_ = KSocketFactory::connectToHost(QLatin1String( "cddbp" ), hostname, port);

    connect (socket_, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(slotGotError(QAbstractSocket::SocketError)));

    connect (socket_, SIGNAL(connected()),
      SLOT(slotConnectionSuccess()) );

    connect (socket_, SIGNAL(readyRead()), SLOT(slotReadyRead()) );

    trackOffsetList_ = trackOffsetList;

    state_ = WaitingForConnection;

    return Success;
  }

    void
  AsyncCDDBPLookup::slotGotError(QAbstractSocket::SocketError error)
  {
    state_ = Idle;

    if ( error == QAbstractSocket::HostNotFoundError )
      emit finished( HostNotFound );
    else if ( error == QAbstractSocket::SocketTimeoutError )
      emit finished( NoResponse );
    else
      emit finished( UnknownError );
  }

    void
  AsyncCDDBPLookup::slotConnectionSuccess()
  {
    kDebug(60010) << "Connection successful";
    state_ = WaitingForGreeting;
  }

    void
  AsyncCDDBPLookup::slotReadyRead()
  {
    kDebug(60010) << "Ready to read. State: " << stateToString();

    while ( Idle != state_ && isConnected() && socket_->canReadLine() )
      read();
  }

    void
  AsyncCDDBPLookup::read()
  {
    switch ( state_ )
    {
      case WaitingForGreeting:

        if ( !parseGreeting( readLine() ) )
        {
          result_ = ServerError;
          doQuit();
          return;
        }

        doHandshake();

        break;

      case WaitingForHandshake:

        if ( !parseHandshake( readLine() ) )
        {
          result_ = ServerError;
          doQuit();
          return;
        }

        doProto();

        break;

      case WaitingForProtoResponse:

        // Ignore the response for now
        readLine();

        doQuery();

        break;

      case WaitingForQueryResponse:
          result_ = parseQuery( readLine() );

          switch ( result_ )
          {
            case Success:
              requestCDInfoForMatch();
              break;

            case MultipleRecordFound:
              state_ = WaitingForMoreMatches;
              break;

            default: // Error :(
              doQuit();
              return;
          }

        break;

      case WaitingForMoreMatches:
        {
          QString line = readLine();

          if (line.startsWith(QLatin1String( "." )))
            requestCDInfoForMatch();
          else
            parseExtraMatch( line );
        }

        break;

      case WaitingForCDInfoResponse:
        {
          Result result = parseRead( readLine() );

          if ( Success != result )
          {
            result_ = result;
            doQuit();
            return;
          }

          state_ = WaitingForCDInfoData;
        }

        break;

      case WaitingForCDInfoData:
        {
          QString line = readLine();

          if (line.startsWith(QLatin1String( "." )))
          {
            parseCDInfoData();
            requestCDInfoForMatch();
          }
          else
            cdInfoBuffer_ << line;
        }

        break;

      case WaitingForQuitResponse:

        state_ = Idle;

        char c;
        while ( socket_->bytesAvailable() )
          socket_->getChar(&c);

        close();

        emit finished( result_ );

        break;

      default:

        break;
    }
  }

    QString
  AsyncCDDBPLookup::readLine()
  {
    return QString::fromUtf8(socket_->readLine());
  }

    void
  AsyncCDDBPLookup::doHandshake()
  {
    sendHandshake();

    state_ = WaitingForHandshake;
  }

    void
  AsyncCDDBPLookup::doProto()
  {
    sendProto();

    state_ = WaitingForProtoResponse;
  }

    void
  AsyncCDDBPLookup::doQuery()
  {
    sendQuery();

    state_ = WaitingForQueryResponse;
  }

    void
  AsyncCDDBPLookup::requestCDInfoForMatch()
  {
    if (matchList_.isEmpty())
    {
      result_ = cdInfoList_.isEmpty()? NoRecordFound : Success;
      doQuit();
      return;
    }

    CDDBMatch match = matchList_.takeFirst();

    sendRead( match );

    state_ = WaitingForCDInfoResponse;
  }

    void
  AsyncCDDBPLookup::parseCDInfoData()
  {
    CDInfo info;

    if (info.load( cdInfoBuffer_ ))
    {
      info.set( QLatin1String( "category" ), category_ );
      info.set( QLatin1String( "discid" ), discid_ );
      info.set( QLatin1String( "source" ), QLatin1String( "freedb" ) );
      cdInfoList_.append( info );
    }

    cdInfoBuffer_.clear();
  }

    void
  AsyncCDDBPLookup::doQuit()
  {
    state_ = WaitingForQuitResponse;

    sendQuit();
  }

    QString
  AsyncCDDBPLookup::stateToString() const
  {
    switch (state_)
    {
      case Idle:
        return QLatin1String( "Idle" );
        break;

      case WaitingForConnection:
        return QLatin1String( "WaitingForConnection" );
        break;

      case WaitingForGreeting:
        return QLatin1String( "WaitingForGreeting" );
        break;

      case WaitingForProtoResponse:
        return QLatin1String( "WaitingForProtoResponse" );
        break;

      case WaitingForHandshake:
        return QLatin1String( "WaitingForHandshake" );
        break;

      case WaitingForQueryResponse:
        return QLatin1String( "WaitingForQueryResponse" );
        break;

      case WaitingForMoreMatches:
        return QLatin1String( "WaitingForMoreMatches" );
        break;

      case WaitingForCDInfoResponse:
        return QLatin1String( "WaitingForCDInfoResponse" );
        break;

      case WaitingForCDInfoData:
        return QLatin1String( "WaitingForCDInfoData" );
        break;

      case WaitingForQuitResponse:
        return QLatin1String( "WaitingForQuitResponse" );
        break;

      default:
        return QLatin1String( "Unknown" );
        break;
    }
  }
}


#include "asynccddbplookup.moc"

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

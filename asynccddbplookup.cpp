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

#include "asynccddbplookup.h"

namespace KCDDB
{
  AsyncCDDBPLookup::AsyncCDDBPLookup(QObject * parent, const char * name)
    : QObject(parent, name),
      CDDBPLookup(), 
      state_(Idle)
  {
    socket_.setBlockingMode( false );
    socket_.enableRead( true );

    socket_.setSocketFlags
      ( socket_.socketFlags() | KExtendedSocket::inputBufferedSocket );

    connect (
        &socket_,
        SIGNAL(lookupFinished(int)),
        SLOT(slotLookupFinished(int))
      );

    connect
      ( 
        &socket_,
        SIGNAL( connectionSuccess() ),
        SLOT( slotConnectionSuccess() )
      );

    connect
      ( 
        &socket_,
        SIGNAL( connectionFailed( int ) ),
        SLOT( slotConnectionFailed( int ) )
      );

    connect
      ( 
        &socket_,
        SIGNAL( readyRead() ),
        SLOT( slotReadyRead() )
      );
  }

  AsyncCDDBPLookup::~AsyncCDDBPLookup()
  {
    close();
  }

    CDDB::Result
  AsyncCDDBPLookup::lookup
  (
    const QString         & hostname,
    uint                    port,
    const QString         & clientName,
    const QString         & clientVersion,
    const TrackOffsetList & trackOffsetList
  )
  {
    if (  trackOffsetList.count() < 3 )
      return UnknownError;

    clientName_ = clientName;
    clientVersion_ = clientVersion;
    trackOffsetList_ = trackOffsetList;

    state_ = WaitingForHostResolution;

    kdDebug(60010) << "Asking socket to connect to "
      << hostname << ":" << port << endl;

    socket_.setAddress( hostname, port );

    if ( 0 != socket_.startAsyncLookup() )
      return HostNotFound;

    return Success;
  }

    void
  AsyncCDDBPLookup::slotLookupFinished( int hostCount )
  {
    kdDebug(60010) << "Found " << hostCount << " hosts" << endl;

    if ( 0 == hostCount )
    {
      emit finished( HostNotFound );
      state_ = Idle;
      return;
    }

    state_ = WaitingForConnection;

    if( 0 != socket_.startAsyncConnect() )
    {
      emit finished( NoResponse );
      state_ = Idle;
    }
  }

    void
  AsyncCDDBPLookup::slotConnectionSuccess()
  {
    kdDebug(60010) << "Connection successful" << endl;
    state_ = WaitingForGreeting;
  }

    void
  AsyncCDDBPLookup::slotConnectionFailed( int err )
  {
    kdDebug(60010) << "Connection failed, error: " << err << endl;
    emit finished( NoResponse );
    state_ = Idle;
    return;
  }

    void
  AsyncCDDBPLookup::slotReadyRead()
  {
    kdDebug(60010) << "Ready to read. State: " << stateToString() << endl;

    while ( Idle != state_ && isConnected() && socket_.canReadLine() )
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

          if ('.' == line[ 0 ])
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

          if ('.' == line[ 0 ])
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

        while ( socket_.bytesAvailable() )
          socket_.getch();
 
        emit finished( result_ );

        break;

      default:

        break;
    }
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

    CDDBMatch match = matchList_.first();
    matchList_.remove( match );

    sendRead( match );

    state_ = WaitingForCDInfoResponse;
  }

    void
  AsyncCDDBPLookup::parseCDInfoData()
  {
    CDInfo info;

    if (info.load( cdInfoBuffer_ ))
      cdInfoList_.append( info );

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
        return "Idle";
        break;

      case WaitingForHostResolution:
        return "WaitingForHostResolution";
        break;

      case WaitingForConnection:
        return "WaitingForConnection";
        break;

      case WaitingForGreeting:
        return "WaitingForGreeting";
        break;

      case WaitingForProtoResponse:
        return "WaitingForProtoResponse";
        break;

      case WaitingForHandshake:
        return "WaitingForHandshake";
        break;

      case WaitingForQueryResponse:
        return "WaitingForQueryResponse";
        break;

      case WaitingForMoreMatches:
        return "WaitingForMoreMatches";
        break;

      case WaitingForCDInfoResponse:
        return "WaitingForCDInfoResponse";
        break;

      case WaitingForCDInfoData:
        return "WaitingForCDInfoData";
        break;

      case WaitingForQuitResponse:
        return "WaitingForQuitResponse";
        break;

      default:
        return "Unknown";
        break;
    }
  }
}


#include "asynccddbplookup.moc"

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

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

#include "asynccddblookup.h"

namespace KCDDB
{
  AsyncCDDBLookup::AsyncCDDBLookup(QObject * parent, const char * name)
    : QObject(parent, name),
      CDDBLookup(), 
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

  AsyncCDDBLookup::~AsyncCDDBLookup()
  {
    // Empty.
  }

    Lookup::Result
  AsyncCDDBLookup::lookup
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

    kdDebug() << "Asking socket to connect to "
      << hostname << ":" << port << endl;

    socket_.setAddress( hostname, port );

    if ( 0 != socket_.startAsyncLookup() )
      return HostNotFound;

    return Success;
  }

    void
  AsyncCDDBLookup::slotLookupFinished( int hostCount )
  {
    kdDebug() << "Found " << hostCount << " hosts" << endl;

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
  AsyncCDDBLookup::slotConnectionSuccess()
  {
    kdDebug() << "Connection successful" << endl;
    state_ = WaitingForGreeting;
  }

    void
  AsyncCDDBLookup::slotConnectionFailed( int err )
  {
    kdDebug() << "Connection failed, error: " << err << endl;
    emit finished( NoResponse );
    state_ = Idle;
    return;
  }

    void
  AsyncCDDBLookup::slotReadyRead()
  {
    kdDebug() << "Ready to read. State: " << stateToString() << endl;

    while ( Idle != state_ && socket_.canReadLine() )
      read();
  }

    void
  AsyncCDDBLookup::read()
  {
    switch (state_)
    {
      case WaitingForGreeting:

        if (!parseGreeting(readLine()))
        {
          emit finished( ServerError );
          state_ = Idle;
          return;
        }

        sendHandshake();

        break;

      case WaitingForHandshake:

        if ( !parseHandshake( readLine() ) )
        {
          emit finished( ServerError );
          state_ = Idle;
          return;
        }

        sendQuery();

        break;

      case WaitingForQueryResponse:
        {
          Result result = parseQuery( readLine() );

          switch (result)
          {
            case Success:
              requestCDInfoForMatch();
              break;

            case MultipleRecordFound:
              state_ = WaitingForMoreMatches;
              break;

            default: // Error :(
              emit finished( result );
              state_ = Idle;
              return;
          }
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
            emit finished( result );
            state_ = Idle;
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

      default:
        break;
    }
  }

    void
  AsyncCDDBLookup::sendHandshake()
  {
    QString line = makeCDDBHandshake();
    writeLine( line );

    state_ = WaitingForHandshake;
  }

    void
  AsyncCDDBLookup::sendQuery()
  {
    QString line = makeCDDBQuery();
    writeLine( line );

    state_ = WaitingForQueryResponse;
  }

    void
  AsyncCDDBLookup::requestCDInfoForMatch()
  {
    if (matchList_.isEmpty())
    {
      if ( cdInfoList_.isEmpty() )
        emit finished( NoRecordFound  );
      else
        emit finished( Success );

      state_ = Idle;

      return;
    }

    CDDBMatch match = matchList_.first();
    matchList_.remove( match );

    // Save genre for later
    CDInfo info;
    info.genre = match.first;
    cdInfoList_.append(info);

    QString line = makeCDDBRead( match );
    writeLine( line );

    state_ = WaitingForCDInfoResponse;
  }

    void
  AsyncCDDBLookup::parseCDInfoData()
  {
    CDInfo info = cdInfoList_.last();
    cdInfoList_.remove( info );
    QString genre = info.genre;

    if (info.load( cdInfoBuffer_ ))
    {
      info.genre = genre;
      cdInfoList_.append( info );
    }

    cdInfoBuffer_.clear();
  }

    QString
  AsyncCDDBLookup::stateToString() const
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

      default:
        return "Unknown";
        break;
    }
  }
}

#include "asynccddblookup.moc"

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

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

#include <qstringlist.h>
#include <kdebug.h>

#include "synccddblookup.h"

namespace KCDDB
{
  SyncCDDBLookup::SyncCDDBLookup()
    : CDDBLookup()
  {
    socket_.setBlockingMode(  false );
  }

  SyncCDDBLookup::~SyncCDDBLookup()
  {
    // Empty.
  }

    Lookup::Result
  SyncCDDBLookup::lookup
  (
    const QString         & hostName,
    uint                    port,
    const QString         & clientName,
    const QString         & clientVersion,
    const TrackOffsetList & trackOffsetList
  )
  {
    if ( trackOffsetList.count() < 3 )
      return UnknownError;

    clientName_ = clientName;
    clientVersion_ = clientVersion;
    trackOffsetList_ = trackOffsetList;

    Result result;

    // Connect to server.
    result = connect( hostName, port );
    if ( Success != result )
      return result;

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

    kdDebug() << matchList_.count() << " matches found." << endl;

    // For each match, read the cd info from the server and save it to
    // cdInfoList.
    CDDBMatchList::ConstIterator matchIt = matchList_.begin();

    while ( matchIt != matchList_.end() )
    {
      CDDBMatch match( *matchIt );
      result = matchToCDInfo( match );
      ++matchIt;
    }

    disconnect();

    return Success;
  }

    Lookup::Result
  SyncCDDBLookup::shakeHands()
  {
    QString line = readLine();

    if ( !parseGreeting( line ) )
      return ServerError;

    line = makeCDDBHandshake();
    writeLine( line );

    line = readLine();

    if ( !parseHandshake( line ) )
      return ServerError;

    return Success;
  }

    Lookup::Result
  SyncCDDBLookup::runQuery()
  {
    Result result;

    QString line = makeCDDBQuery();
    writeLine( line );

    line = readLine();
    result = parseQuery( line );

    if ( ServerError == result )
      return ServerError;

    if ( MultipleRecordFound == result )
    {
      // We have multiple matches
      line = readLine();

      while ( '.' != line[ 0 ] )
      {
        parseExtraMatch( line );
        line = readLine();
      }
    }
 
    return Success;
  }

    Lookup::Result
  SyncCDDBLookup::matchToCDInfo( const CDDBMatch & match )
  {
    QString line = makeCDDBRead( match );
    writeLine( line );

    line = readLine();

    Result result = parseRead( line );
    if ( Success != result )
      return result;

    QStringList lineList;
    line = readLine();

    while ( '.' != line[0] )
    {
      lineList.append( line );
      line = readLine();
    }

    CDInfo info;

    if ( info.load( lineList ) )
    {
      info.genre = match.first;
      cdInfoList_.append( info );
    }

    return Success;
  }

    Lookup::Result
  SyncCDDBLookup::connect( const QString & hostName, uint port )
  {
    kdDebug() << "Trying to connect to " << hostName << ":" << port << endl;

    if ( !socket_.setAddress( hostName, port ) )
      return UnknownError;

    socket_.setTimeout( 30 );

    if ( 0 != socket_.lookup() )
      return HostNotFound;

    if ( 0 != socket_.connect() )
      return NoResponse;

    kdDebug() << "Connected" << endl;
    return Success;
  }

    void 
  SyncCDDBLookup::disconnect()
  {
    writeLine("quit");
    socket_.close();
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

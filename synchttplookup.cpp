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
#include <qapplication.h>

#include <kdebug.h>
#include <kio/job.h>

#include "synchttplookup.h"

namespace KCDDB
{
  SyncHTTPLookup::SyncHTTPLookup()
    : HTTPLookup(), done_( false )
  {
    socket_.setBlockingMode(  false );
  }

  SyncHTTPLookup::~SyncHTTPLookup()
  {
    // Empty.
  }

    Lookup::Result
  SyncHTTPLookup::lookup
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

    initURL( hostName, port );

    // Run a query.
    result = runQuery();
    if ( Success != result )
      return result;

    kdDebug() << matchList_.count() << " matches found." << endl;

    if (matchList_.isEmpty())
      return NoRecordFound;

    // For each match, read the cd info from the server and save it to
    // cdInfoList.
    CDDBMatchList::ConstIterator matchIt = matchList_.begin();

    while ( matchIt != matchList_.end() )
    {
      CDDBMatch match( *matchIt );
      result = matchToCDInfo( match );
      ++matchIt;
    }

    return Success;
  }

    Lookup::Result
  SyncHTTPLookup::runQuery()
  {
    done_ = false;
    data_ = QString::null;
    state_ = WaitingForQueryResponse;

    makeQueryURL();

    result_ = submitJob();

    if ( Success != result_ )
      return result_;

    while ( !done_ )
      qApp->processOneEvent();

    kdDebug() << "runQuery() Result: " << resultToString(result_) << endl;

    return result_;
  }

    Lookup::Result
  SyncHTTPLookup::matchToCDInfo( const CDDBMatch & match )
  {
    done_ = false;
    data_ = QString::null;
    state_ = WaitingForReadResponse;

    makeReadURL( match );

    result_ = submitJob();

    if ( Success != result_ )
      return result_;

    while ( !done_ )
      qApp->processOneEvent();

    return result_;
  }

    void
  SyncHTTPLookup::slotResult(  KIO::Job *job )
  {
    done_ = true;

    HTTPLookup::slotResult( job );
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

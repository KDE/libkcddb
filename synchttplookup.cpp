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
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#include "synchttplookup.h"

#include <qstringlist.h>
#include <qapplication.h>

#include <kdebug.h>
#include <kio/job.h>
#include <kio/netaccess.h>

namespace KCDDB
{
  SyncHTTPLookup::SyncHTTPLookup()
    : HTTPLookup()
  {
  }

  SyncHTTPLookup::~SyncHTTPLookup()
  {
    // Empty.
  }

    Result
  SyncHTTPLookup::lookup
  (
    const QString         & hostName,
    uint                    port,
    const TrackOffsetList & trackOffsetList
  )
  {
    trackOffsetList_ = trackOffsetList;

    initURL( hostName, port );

    // Run a query.
    result_ = runQuery();

    if ( Success != result_ )
      return result_;

    kDebug(60010) << matchList_.count() << " matches found.";

    if (matchList_.isEmpty())
      return NoRecordFound;

    // For each match, read the cd info from the server and save it to
    // cdInfoList.
    CDDBMatchList::ConstIterator matchIt = matchList_.constBegin();

    while ( matchIt != matchList_.constEnd() )
    {
      CDDBMatch match( *matchIt );
      result_ = matchToCDInfo( match );
      ++matchIt;
    }

    return result_;
  }

    Result
  SyncHTTPLookup::runQuery()
  {
    data_ = QByteArray();
    state_ = WaitingForQueryResponse;

    result_ = sendQuery();

    if ( Success != result_ )
      return result_;

    kDebug(60010) << "runQuery() Result: " << resultToString(result_);

    return result_;
  }

    Result
  SyncHTTPLookup::matchToCDInfo( const CDDBMatch & match )
  {
    data_ = QByteArray();
    state_ = WaitingForReadResponse;

    result_ = sendRead( match );

    if ( Success != result_ )
      return result_;

    return result_;
  }

    Result
  SyncHTTPLookup::fetchURL()
  {
    kDebug(60010) << "About to fetch: " << cgiURL_.url();

    KIO::TransferJob* job = KIO::get( cgiURL_, KIO::NoReload, KIO::HideProgressInfo );

    if ( 0 == job )
      return ServerError;

    if (!KIO::NetAccess::synchronousRun(job, 0, &data_))
      return ServerError;

    jobFinished();

    return Success;
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

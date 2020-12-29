/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "synchttplookup.h"
#include "logging.h"

#include <KIO/Job>

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

	qCDebug(LIBKCDDB) << matchList_.count() << " matches found.";

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

	qCDebug(LIBKCDDB) << "runQuery() Result: " << resultToString(result_);

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
	qCDebug(LIBKCDDB) << "About to fetch: " << cgiURL_.url();

    KIO::TransferJob* job = KIO::get( cgiURL_, KIO::NoReload, KIO::HideProgressInfo );

    if ( nullptr == job )
      return ServerError;

    QObject::connect( job, &KIO::TransferJob::data, [&](KIO::Job *, const QByteArray &data){ data_ += data; } );

    if (!job->exec())
      return ServerError;

    jobFinished();

    return Success;
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

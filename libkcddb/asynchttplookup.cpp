/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "asynchttplookup.h"
#include "logging.h"

#include <KIO/TransferJob>

namespace KCDDB
{
  AsyncHTTPLookup::AsyncHTTPLookup()
    : HTTPLookup()
  {
    block_ = false;
  }

  AsyncHTTPLookup::~AsyncHTTPLookup()
  {
    // Empty.
  }

    Result
  AsyncHTTPLookup::lookup
  (
    const QString         & hostName,
    uint                    port,
    const TrackOffsetList & trackOffsetList
  )
  {
    trackOffsetList_ = trackOffsetList;

    connect( this, &HTTPLookup::queryReady, this, &AsyncHTTPLookup::slotQueryReady );
    connect( this, &HTTPLookup::readReady, this, &AsyncHTTPLookup::requestCDInfoForMatch );

    initURL( hostName, port );

    // Run a query.
    result_ = runQuery();

    return result_;
  }

    Result
  AsyncHTTPLookup::runQuery()
  {
    data_ = QByteArray();
    state_ = WaitingForQueryResponse;

    result_ = sendQuery();

    return result_;
  }

    void
  AsyncHTTPLookup::slotQueryReady()
  {
	qCDebug(LIBKCDDB) << "Matches Found: " <<  matchList_.count();

    if ( Success != result_ )
    {
      Q_EMIT finished( result_ );
      return;
    }

    requestCDInfoForMatch();
  }

    void
  AsyncHTTPLookup::requestCDInfoForMatch()
  {
    if ( matchList_.isEmpty() )
    {
      result_ = cdInfoList_.isEmpty()? NoRecordFound : Success;
      Q_EMIT finished( result_ );
      return;
    }

    CDDBMatch match = matchList_.takeFirst();

    data_ = QByteArray();
    state_ = WaitingForReadResponse;

    result_ = sendRead( match );

    if ( Success != result_ )
      Q_EMIT finished( result_ );
  }

    void
  AsyncHTTPLookup::slotData( KIO::Job *, const QByteArray &data )
  {
    if (data.size() > 0)
      data_.append(data);
  }

    void
  AsyncHTTPLookup::slotResult( KJob *job )
  {
    if ( 0 != job->error() )
    {
      result_ = ServerError;
      if ( !block_ )
        Q_EMIT queryReady();
      return;
    }

    jobFinished();
  }

    Result
  AsyncHTTPLookup::fetchURL()
  {
	qCDebug(LIBKCDDB) << "About to fetch: " << cgiURL_.url();

    KIO::TransferJob* job = KIO::get( cgiURL_, KIO::NoReload, KIO::HideProgressInfo );

    if ( nullptr == job )
      return ServerError;

    connect( job, &KIO::TransferJob::data,
          this, &AsyncHTTPLookup::slotData );
    connect( job, &KJob::result,
          this, &AsyncHTTPLookup::slotResult );

    return Success;
  }

}

#include "moc_asynchttplookup.cpp"

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

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

#include "asynchttplookup.h"

#include <qstringlist.h>
#include <qapplication.h>

#include <kdebug.h>
#include <kio/job.h>

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

    connect( this, SIGNAL( queryReady() ), SLOT( slotQueryReady() ) );
    connect( this, SIGNAL( readReady() ), SLOT( requestCDInfoForMatch() ) );

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
    kDebug(60010) << "Matches Found: " <<  matchList_.count();

    if ( Success != result_ )
    {
      emit finished( result_ );
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
      emit finished( result_ );
      return;
    }

    CDDBMatch match = matchList_.takeFirst();

    data_ = QByteArray();
    state_ = WaitingForReadResponse;

    result_ = sendRead( match );

    if ( Success != result_ )
      emit finished( result_ );
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
        emit queryReady();
      return;
    }

    jobFinished();
  }

    Result
  AsyncHTTPLookup::fetchURL()
  {
    kDebug(60010) << "About to fetch: " << cgiURL_.url();

    KIO::TransferJob* job = KIO::get( cgiURL_, KIO::NoReload, KIO::HideProgressInfo );

    if ( 0 == job )
      return ServerError;

    connect( job, SIGNAL( data( KIO::Job *, const QByteArray & ) ),
          SLOT( slotData( KIO::Job *, const QByteArray & ) ) );
    connect( job, SIGNAL( result( KJob * ) ),
          SLOT( slotResult( KJob * ) ) );

    return Success;
  }

}

#include "asynchttplookup.moc"

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

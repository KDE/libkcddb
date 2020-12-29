/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCDDB_ASYNC_HTTP_LOOKUP_H
#define KCDDB_ASYNC_HTTP_LOOKUP_H

#include "httplookup.h"

class KJob;

namespace KCDDB
{
  class AsyncHTTPLookup : public HTTPLookup
  {

    Q_OBJECT

    public:

      AsyncHTTPLookup();
      virtual ~AsyncHTTPLookup();

      Result lookup( const QString &, uint, const TrackOffsetList & ) override;

      CDInfoList lookupResponse() const;

    Q_SIGNALS:

      void finished( KCDDB::Result );

    protected Q_SLOTS:
      void slotQueryReady();
      void requestCDInfoForMatch();

      void slotData( KIO::Job *, const QByteArray & );
      void slotResult( KJob * );

    protected:
      Result fetchURL() override;

      Result runQuery();
  };
}

#endif // KCDDB_ASYNC_HTTP_LOOKUP_H

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

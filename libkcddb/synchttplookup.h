/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCDDB_SYNC_HTTP_LOOKUP_H
#define KCDDB_SYNC_HTTP_LOOKUP_H

#include "httplookup.h"

namespace KCDDB
{
  class SyncHTTPLookup : public HTTPLookup
  {
    public:

      SyncHTTPLookup();
      virtual ~SyncHTTPLookup();

      Result lookup( const QString &, uint, const TrackOffsetList & ) override;

      CDInfoList lookupResponse() const;

    protected:

      Result fetchURL() override;

      Result runQuery();
      Result matchToCDInfo( const CDDBMatch & );
  };
}

#endif // KCDDB_SYNC_HTTP_LOOKUP_H

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

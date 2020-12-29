/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>
    SPDX-FileCopyrightText: 2002 Nadeem Hasan <nhasan@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCDDB_SYNC_CDDBP_LOOKUP_H
#define KCDDB_SYNC_CDDBP_LOOKUP_H

#include "cddbplookup.h"

namespace KCDDB
{
  class SyncCDDBPLookup : public CDDBPLookup
  {
    public:

      SyncCDDBPLookup();
      virtual ~SyncCDDBPLookup();

      Result lookup( const QString &, uint, const TrackOffsetList & ) override;

      CDInfoList lookupResponse() const;

    protected:
      Result shakeHands();
      Result runQuery();
      Result matchToCDInfo( const CDDBMatch & );

      QString readLine();
  };
}

#endif // KCDDB_SYNC_CDDBP_LOOKUP_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002 Benjamin Meyer <ben-devel@meyerhome.net>
  Copyright (C) 2002 Nadeem Hasan <nhasan@kde.org>

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
  the Free Software Foundation, Inc., 51 Franklin Steet, Fifth Floor,
  Boston, MA 02110-1301, USA.
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

      Result lookup( const QString &, uint, const TrackOffsetList & );

      CDInfoList lookupResponse() const;

    protected:
      Result connect();
      Result shakeHands();
      Result runQuery();
      Result matchToCDInfo( const CDDBMatch & );

      QString readLine();
  };
}

#endif // KCDDB_SYNC_CDDBP_LOOKUP_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

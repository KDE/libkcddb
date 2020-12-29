/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>
    SPDX-FileCopyrightText: 2002 Nadeem Hasan <nhasan@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCDDB_CACHE_H
#define KCDDB_CACHE_H

#include "kcddb.h"
#include "cdinfo.h"

#include <QString>

namespace KCDDB
{
  class Config;
  
  class KCDDB_EXPORT Cache
  {
    public:
      static CDInfoList lookup( const TrackOffsetList & , const Config & );
      static void store( const TrackOffsetList &, const CDInfoList &, const Config & );
      static void store( const TrackOffsetList &, const CDInfo &, const Config & );

    private:
      static QString fileName( const QString &category, const QString& discid, const QString &cacheDir );
  };
}

#endif // KCDDB_CACHE_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

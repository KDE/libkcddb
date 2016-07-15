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
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#ifndef KCDDB_CACHE_H
#define KCDDB_CACHE_H

#include "kcddb.h"
#include "cdinfo.h"

#include <QtCore/QString>

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

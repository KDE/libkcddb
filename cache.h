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
  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA 02111-1307, USA.
*/

#ifndef KCDDB_CACHE_H
#define KCDDB_CACHE_H

#include <qpair.h>
#include <qstring.h>

#include "cdinfo.h"

namespace KCDDB
{
  class Cache
  {
    public:

      enum Policy
      {
        Only,
        Use,
        Ignore
      };

      static CDInfoList lookup( const QString & );
      static void store( const CDInfoList & );

    private:
      static QString fileName( const CDInfo & );
  };
}

#endif // KCDDB_CACHE_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

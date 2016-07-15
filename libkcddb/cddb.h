/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002 Benjamin Meyer <ben-devel@meyerhome.net>
  CopyRight (C) 2002 Nadeem Hasan <nhasan@kde.org>

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

#ifndef KCDDB_CDDB_H
#define KCDDB_CDDB_H

#include <libkcddb/kcddb.h>
#include <libkcddb/cdinfo.h>
#include <libkcddb/kcddbconfig.h>

#include <QtCore/QList>
#include <QtCore/QString>

namespace KCDDB
{
  class CDDB
  {
    public:
      CDDB();
      virtual ~CDDB();

      static QString trackOffsetListToId( const TrackOffsetList & );

      static QString clientName() { return QLatin1String("libkcddb"); }
      static QString clientVersion() { return QLatin1String("0.5"); }

      static uint statusCode( const QString & );

      static CDInfoList cacheFiles(const TrackOffsetList &, const Config& );

    protected:
      QString trackOffsetListToId();
      QString trackOffsetListToString();
 
      QString user_;
      QString localHostName_;

      bool readOnly_;

      TrackOffsetList trackOffsetList_;
  };
}

#endif // KCDDB_CDDB_H

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

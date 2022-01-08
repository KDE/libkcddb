/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>
    SPDX-FileCopyrightText: 2002 Nadeem Hasan <nhasan@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCDDB_CDDB_H
#define KCDDB_CDDB_H

#include "kcddb.h"
#include "cdinfo.h"
#include "config.h"

#include <QList>
#include <QString>

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

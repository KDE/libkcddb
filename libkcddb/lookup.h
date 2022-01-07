/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>
    SPDX-FileCopyrightText: 2002 Nadeem Hasan <nhasan@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCDDB_LOOKUP_H
#define KCDDB_LOOKUP_H

#include "cddb.h"
#include "cdinfo.h"

#include <QList>
#include <QObject>
#include <QPair>

namespace KCDDB
{
  typedef QPair<QString, QString> CDDBMatch;
  typedef QList<CDDBMatch> CDDBMatchList;

  class Lookup : public CDDB, public QObject
  {
    public:

      enum Transport
      {
        CDDBP,
        HTTP
      };


      Lookup();
      virtual ~Lookup();

      virtual Result lookup( const QString &, uint, const TrackOffsetList & ) = 0;

      CDInfoList lookupResponse() const;

    protected:

      void parseExtraMatch(  const QString & );
      Result parseQuery(  const QString & );
      Result parseRead(  const QString & );

      CDInfoList cdInfoList_;
      CDDBMatchList matchList_;
      QString category_;
      QString discid_;
  };
}

#endif // KCDDB_LOOKUP_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

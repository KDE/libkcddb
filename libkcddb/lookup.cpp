/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>
    SPDX-FileCopyrightText: 2002 Nadeem Hasan <nhasan@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "lookup.h"

#include <QDebug>

namespace KCDDB
{
  Lookup::Lookup()
     : CDDB()
  {
  }

  Lookup::~Lookup()
  {
    // Empty.
  }

    Result
  Lookup::parseQuery(  const QString & line )
  {
    uint serverStatus = statusCode(  line );

    if (  200 == serverStatus )
    {
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
      QStringList tokenList = line.split( QLatin1Char( ' ' ), QString::SkipEmptyParts );
#else
      QStringList tokenList = line.split( QLatin1Char( ' ' ), Qt::SkipEmptyParts );
#endif
      matchList_.append(  qMakePair(  tokenList[  1 ], tokenList[  2 ] ) );
      return Success;
    }
    else if (  (  211 == serverStatus ) || (  210 == serverStatus ) )
    {
      return MultipleRecordFound;
    }
    else if (  202 == serverStatus )
    {
      return NoRecordFound;
    }

    return ServerError;
  }

    void
  Lookup::parseExtraMatch(  const QString & line )
  {
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    QStringList tokenList = line.split( QLatin1Char( ' ' ), QString::SkipEmptyParts );
#else
    QStringList tokenList = line.split( QLatin1Char( ' ' ), Qt::SkipEmptyParts );
#endif
    matchList_.append(  qMakePair(  tokenList[  0 ], tokenList[  1 ] ) );
  }

    Result
  Lookup::parseRead(  const QString & line )
  {
    uint serverStatus = statusCode(  line );

    if (   210 != serverStatus )
      return ServerError;

    return Success;
  }

    CDInfoList
  Lookup::lookupResponse() const
  {
    return cdInfoList_;
  }

}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

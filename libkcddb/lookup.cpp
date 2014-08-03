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

#include "lookup.h"

#include <QtCore/QDebug>

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
      QStringList tokenList = line.split( QLatin1Char( ' ' ), QString::SkipEmptyParts );
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
    QStringList tokenList = line.split( QLatin1Char( ' ' ), QString::SkipEmptyParts );
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

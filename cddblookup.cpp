/*
  Copyright ( C ) 2002 Rik Hemsley (  rikkus ) <rik@kde.org>
  Copyright ( C ) 2002 Benjamin Meyer <ben-devel@meyerhome.net>
  Copyright ( C ) 2002 Nadeem Hasan <nhasan@kde.org>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (  at your option ) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to
  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA 02111-1307, USA.
*/

#include "cddblookup.h"

namespace KCDDB
{
  CDDBLookup::CDDBLookup()
    : Lookup()
  {
  }

  CDDBLookup::~CDDBLookup()
  {
  }

    QString
  CDDBLookup::makeHandshakeCommand()
  {
    QString handshake = QString( "cddb hello %1 %2 %3 %4" )
        .arg( user_ )
        .arg( localHostName_ )
        .arg( clientName_ )
        .arg( clientVersion_ );

    return handshake;
  }

    QString
  CDDBLookup::makeProtoCommand()
  {
    return "proto 5";
  }

    QString
  CDDBLookup::makeQueryCommand()
  {
    QString query = QString( "cddb query %1 %2" )
        .arg( trackOffsetListToId() )
        .arg( trackOffsetListToString() );

    return query;
  }

    QString
  CDDBLookup::makeReadCommand( const CDDBMatch & match )
  {
    QString category  = match.first;
    QString discid    = match.second;

    QString readRequest = QString( "cddb read %1 %2" )
        .arg( category )
        .arg( discid );

    return readRequest;
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

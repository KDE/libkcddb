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
  {
  }

  CDDBLookup::~CDDBLookup()
  {
  }

    QString
  CDDBLookup::makeCDDBHandshake()
  {
    QString handshake = "cddb hello ";
    handshake += "libkcddb-user";
    handshake += " ";
    handshake += "localhost"; // FIXME
    handshake += " ";
    handshake += clientName_;
    handshake += " ";
    handshake += clientVersion_;

    return handshake;
  }

    QString
  CDDBLookup::makeCDDBQuery( const TrackOffsetList & trackOffsetList )
  {
    QString query = "cddb query ";
    query += trackOffsetListToId( trackOffsetList );
    query += " ";
    query += trackOffsetListToString( trackOffsetList );

    return query;
  }

    QString
  CDDBLookup::makeCDDBRead( const CDDBMatch & match )
  {
    QString category  = match.first;
    QString discid    = match.second;

    QString readRequest = "cddb read ";
    readRequest += category;
    readRequest += " ";
    readRequest += discid;

    return readRequest;
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

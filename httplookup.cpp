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

#include "httplookup.h"

namespace KCDDB
{
  HTTPLookup::HTTPLookup()
    : Lookup()
  {
  }

  HTTPLookup::~HTTPLookup()
  {
  }

    void
  HTTPLookup::makeQueryURL()
  {
    QString cmd = QString( "cddb query %1 %2" )
      .arg( trackOffsetListToId() )
      .arg( trackOffsetListToString() ) ;

    makeURL( cmd );

    return;
  }

    void
  HTTPLookup::makeReadURL( const CDDBMatch & match )
  {
    QString category  = match.first;
    QString discid    = match.second;

    QString cmd = QString( "cddb read %1 %2" )
        .arg( category )
        .arg( discid );

    makeURL( cmd );

    return;
  }

    void
  HTTPLookup::initURL( const QString & hostName, uint port )
  {
    cgiURL_.setProtocol( "http" );
    cgiURL_.setHost( hostName );
    cgiURL_.setPort( port );
    cgiURL_.setPath( "/~cddb/cddb.cgi" );

    return;
  }

    void
  HTTPLookup::makeURL( const QString & cmd )
  {
    // The whole query has to constructed each time as the
    // CDDB CGI script expects the parameters in strict order

    cgiURL_.setQuery( QString::null );

    QString hello = QString( "%1 %2 %3 %4" )
      .arg( user_ )
      .arg( localHostName_ )
      .arg( clientName_ )
      .arg( clientVersion_ );

    cgiURL_.addQueryItem( "cmd", cmd );
    cgiURL_.addQueryItem( "hello", hello );
    cgiURL_.addQueryItem( "proto", "5" );
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

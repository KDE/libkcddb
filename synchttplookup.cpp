/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002 Benjamin Meyer <ben-devel@meyerhome.net>

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

#include <qstringlist.h>
#include <qfile.h>

#include <kdebug.h>
#include <kio/netaccess.h>

#include "synchttplookup.h"

namespace KCDDB
{
  SyncHTTPLookup::SyncHTTPLookup()
    : HTTPLookup()
  {
    socket_.setBlockingMode(  false );
  }

  SyncHTTPLookup::~SyncHTTPLookup()
  {
    // Empty.
  }

    Lookup::Result
  SyncHTTPLookup::lookup
  (
    const QString         & hostName,
    uint                    port,
    const QString         & clientName,
    const QString         & clientVersion,
    const TrackOffsetList & trackOffsetList
  )
  {
    if ( trackOffsetList.count() < 3 )
      return UnknownError;

    clientName_ = clientName;
    clientVersion_ = clientVersion;
    trackOffsetList_ = trackOffsetList;

    Result result;

    initURL( hostName, port );

    // Run a query.
    result = runQuery();
    if ( Success != result )
      return result;

    if (matchList_.isEmpty())
      return NoRecordFound;

    kdDebug() << matchList_.count() << " matches found." << endl;

    // For each match, read the cd info from the server and save it to
    // cdInfoList.
    CDDBMatchList::ConstIterator matchIt = matchList_.begin();

    while ( matchIt != matchList_.end() )
    {
      CDDBMatch match( *matchIt );
      result = matchToCDInfo( match );
      ++matchIt;
    }

    return Success;
  }

    Lookup::Result
  SyncHTTPLookup::runQuery()
  {
    Result result;
    QString target;

    makeQueryURL();

    kdDebug() << "About to fetch: " << cgiURL_.url() << endl;

    if ( !KIO::NetAccess::download( cgiURL_, target ) )
      return ServerError;

    QFile f( target );

    if ( !f.open( IO_ReadOnly ) )
    {
      KIO::NetAccess::removeTempFile( target );
      return ServerError;
    }

    QTextStream ts( &f );

    QString line = ts.readLine();
    result = parseQuery( line );

    if ( ServerError == result )
    {
      KIO::NetAccess::removeTempFile( target );
      return ServerError;
    }

    if ( MultipleRecordFound == result )
    {
      // We have multiple matches
      line = ts.readLine();

      while ( '.' != line[ 0 ] )
      {
        parseExtraMatch( line );
        line = readLine();
      }
    }
 
    f.close();

    KIO::NetAccess::removeTempFile( target );

    return Success;
  }

    Lookup::Result
  SyncHTTPLookup::matchToCDInfo( const CDDBMatch & match )
  {
    Result result;
    QString target;

    makeReadURL( match );

    kdDebug() << "About to fetch: " << cgiURL_.url() << endl;

    if ( !KIO::NetAccess::download( cgiURL_, target ) )
      return ServerError;

    QFile f( target );

    if ( !f.open( IO_ReadOnly ) )
    {
      KIO::NetAccess::removeTempFile( target );
      return ServerError;
    }

    QTextStream ts( &f );

    QString line = ts.readLine();
    result = parseRead( line );

    if ( Success != result )
    {
      KIO::NetAccess::removeTempFile( target );
      return result;
    }

    QStringList lineList;
    line = ts.readLine();

    while ( '.' != line[0] )
    {
      lineList.append( line );
      line = readLine();
    }

    CDInfo info;

    if ( info.load( lineList ) )
      cdInfoList_.append( info );

    return Success;
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

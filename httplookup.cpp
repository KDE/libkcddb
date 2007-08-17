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
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#include "httplookup.h"

#include <kio/job.h>
#include <kdebug.h>

namespace KCDDB
{
  HTTPLookup::HTTPLookup()
    : Lookup(),
      block_( true ), state_( Idle ), result_( Success )
  {
  }

  HTTPLookup::~HTTPLookup()
  {
  }

    Result
  HTTPLookup::sendQuery()
  {
    QString cmd = QString( "cddb query %1 %2" )
      .arg( trackOffsetListToId(), trackOffsetListToString() ) ;

    makeURL( cmd );
    Result result = fetchURL();

    return result;
  }

    Result
  HTTPLookup::sendRead( const CDDBMatch & match )
  {
    category_  = match.first;
    discid_    = match.second;

    QString cmd = QString( "cddb read %1 %2" )
        .arg( category_, discid_ );

    makeURL( cmd );
    Result result = fetchURL();

    return result;
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

    cgiURL_.setQuery( QString::null );	//krazy:exclude=nullstrassign for old broken gcc

    QString hello = QString("%1 %2 %3 %4")
        .arg(user_, localHostName_, clientName(), clientVersion());

    cgiURL_.addQueryItem( "cmd", cmd );
    cgiURL_.addQueryItem( "hello", hello );
    cgiURL_.addQueryItem( "proto", "6" );
  }

    void
  HTTPLookup::jobFinished()
  {
    QStringList lineList = QString::fromUtf8(data_, data_.size()).split( "\n", QString::SkipEmptyParts );
    QStringList::ConstIterator it = lineList.begin();

    switch ( state_ )
    {
      case WaitingForQueryResponse:

        if ( it != lineList.end() )
        {
          QString line( *it );

          result_ = parseQuery( line );

          switch ( result_ )
          {
            case Success:

              if ( !block_ )
                emit queryReady();
              break;

            case MultipleRecordFound:

              ++it;
              while ( it != lineList.end() )
              {
                QString line( *it );

                if ( '.' == line[ 0 ] )
                {
                  result_ = Success;

                  if ( !block_ )
                    emit queryReady();
                  break;
                }

                parseExtraMatch( line );
                ++it;
              }

              break;

            case ServerError:
            case NoRecordFound:
              if ( !block_ )
                emit queryReady();

              return;
              break;

            default:

              break;
          }

        }

        break;

      case WaitingForReadResponse:

        {
          CDInfo info;

          if ( info.load( QString::fromUtf8(data_,data_.size()) ) )
          {
            info.set( "category", category_ );
            info.set( "discid", discid_ );
            info.set( "source", "freedb" );
            cdInfoList_.append( info );
          }

          if ( !block_ )
            emit readReady();
        }

        return;

        break;

      default:

        break;
    }

    result_ = Success;
  }
}

#include "httplookup.moc"

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

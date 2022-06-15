/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (  rikkus ) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>
    SPDX-FileCopyrightText: 2002 Nadeem Hasan <nhasan@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "httplookup.h"

#include <KIO/Job>
#include <QDebug>
#include <QUrlQuery>

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
      QString cmd = QString::fromLatin1( "cddb query %1 %2" )
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

    QString cmd = QString::fromLatin1( "cddb read %1 %2" )
        .arg( category_, discid_ );

    makeURL( cmd );
    Result result = fetchURL();

    return result;
  }

    void
  HTTPLookup::initURL( const QString & hostName, uint port )
  {
    cgiURL_.setScheme( QLatin1String( "http" ) );
    cgiURL_.setHost( hostName );
    cgiURL_.setPort( port );
    cgiURL_.setPath( QLatin1String( "/~cddb/cddb.cgi" ) );

    return;
  }

    void
  HTTPLookup::makeURL( const QString & cmd )
  {
    QString hello = QString::fromLatin1("%1 %2 %3 %4")
        .arg(user_, localHostName_, clientName(), clientVersion());

    // The whole query has to constructed each time as the
    // CDDB CGI script expects the parameters in strict order
    QUrlQuery query;
    query.addQueryItem( QLatin1String( "cmd" ), cmd );
    query.addQueryItem( QLatin1String( "hello" ), hello );
    query.addQueryItem( QLatin1String( "proto" ), QLatin1String( "6" ) );
    cgiURL_.setQuery( query );
  }

    void
  HTTPLookup::jobFinished()
  {
    QStringList lineList = QString::fromUtf8(data_).split( QLatin1String( "\n" ), Qt::SkipEmptyParts );
    QStringList::ConstIterator it = lineList.constBegin();

    switch ( state_ )
    {
      case WaitingForQueryResponse:

        if ( it != lineList.constEnd() )
        {
          QString line( *it );

          result_ = parseQuery( line );

          switch ( result_ )
          {
            case Success:

              if ( !block_ )
                Q_EMIT queryReady();
              break;

            case MultipleRecordFound:

              ++it;
              while ( it != lineList.constEnd() )
              {
                QString line( *it );

                if ( QLatin1Char( '.' ) == line[ 0 ] )
                {
                  result_ = Success;

                  if ( !block_ )
                    Q_EMIT queryReady();
                  break;
                }

                parseExtraMatch( line );
                ++it;
              }

              break;

            case ServerError:
            case NoRecordFound:
              if ( !block_ )
                Q_EMIT queryReady();

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

          if ( info.load( QString::fromUtf8(data_) ) )
          {
            info.set( QLatin1String( "category" ), category_ );
            info.set( QLatin1String( "discid" ), discid_ );
            info.set( QLatin1String( "source" ), QLatin1String( "freedb" ) );
            cdInfoList_.append( info );
          }

          if ( !block_ )
            Q_EMIT readReady();
        }

        return;

        break;

      default:

        break;
    }

    result_ = Success;
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

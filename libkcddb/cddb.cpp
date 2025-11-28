/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>
    SPDX-FileCopyrightText: 2002 Nadeem Hasan <nhasan@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "cddb.h"

#include "categories.h"
#include "kcddbi18n.h"


#include <QStringList>

namespace KCDDB
{
  CDDB::CDDB()
    : user_( QLatin1String( "libkcddb-user" ) ),
      localHostName_( QLatin1String( "localHost" ) ),
      readOnly_( false )
  {

  }

  CDDB::~CDDB()
  {
    // Empty.
  }

    QString
  CDDB::trackOffsetListToId()
  {
    return trackOffsetListToId( trackOffsetList_ );
  }
    QString
  CDDB::trackOffsetListToId( const TrackOffsetList & list )
  {
    // Taken from version by Michael Matz in kio_audiocd.
    unsigned int id = 0;
    if ( list.isEmpty() )
        return QString();
    int numTracks = list.count() - 1;

    // The last two in the list are disc begin and disc end.
    for ( int i = numTracks-1; i >= 0; i-- )
    {
      int n = list[ i ]/75;
      while ( n > 0 )
      {
        id += n % 10;
        n /= 10;
      }
    }

    unsigned int l = list[numTracks] / 75;
    l -= list[0] / 75;

    id = ( ( id % 255 ) << 24 ) | ( l << 8 ) | numTracks;

    return QString::number( id, 16 ).rightJustified( 8, QLatin1Char( '0' ) );
  }

    QString
  CDDB::trackOffsetListToString()
  {
    QString ret;
    uint numTracks = trackOffsetList_.count()-1;

    // Disc start.
    ret.append( QString::number( numTracks ) );
    ret.append( QLatin1String( " " ) );

    for ( uint i = 0; i < numTracks; i++ )
    {
      ret.append( QString::number( trackOffsetList_[ i ] ) );
      ret.append( QLatin1String( " " ) );
    }

    unsigned int discLengthInSec = ( trackOffsetList_[ numTracks ] ) / 75;

    ret.append( QString::number( discLengthInSec ) );

    return ret;
  }

    uint
  CDDB::statusCode( const QString & line )
  {
    const QStringList tokenList = line.split(QLatin1Char( ' ' ), Qt::SkipEmptyParts );

    if (tokenList.isEmpty())
        return 410;

    return tokenList[ 0 ].toUInt();
  }

    CDInfoList
  CDDB::cacheFiles(const TrackOffsetList &offsetList, const Config& config )
  {
    Categories c;
    QStringList categories = c.cddbList();
    // Also load user-created entries
    categories << QLatin1String( "user" );

    CDInfoList infoList;
    QStringList cddbCacheDirs = config.cacheLocations();

    for (QStringList::const_iterator cddbCacheDir = cddbCacheDirs.constBegin();
        cddbCacheDir != cddbCacheDirs.constEnd(); ++cddbCacheDir)
    {
      for (const QString &category : std::as_const(categories)) {
        QFile f( *cddbCacheDir + QLatin1Char( '/' ) + category + QLatin1Char( '/' ) + trackOffsetListToId(offsetList) );
        if ( f.exists() && f.open(QIODevice::ReadOnly) )
        {
            QTextStream ts(&f);
            QString cddbData = ts.readAll();
            f.close();
            CDInfo info;
            info.load(cddbData);
            if (category != QLatin1String( "user" ))
            {
              info.set(Category,category);
              info.set(QLatin1String( "source" ), QLatin1String( "freedb" ));
            }
            else
            {
              info.set(QLatin1String( "source" ), QLatin1String( "user" ));
            }

            infoList.append( info );
        }
      }
    }

    return infoList;
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

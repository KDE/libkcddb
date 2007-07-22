/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002 Benjamin Meyer <ben-devel@meyerhome.net>
  Copyright (C) 2002 Nadeem Hasan <nhasan@kde.org>
  Copyright (C) 2007 Richard Lärkäng <nouseforaname@home.se>

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

#include "cache.h"

#include "kcddbconfig.h"
#include "cddb.h"

#include <kdebug.h>
#include <kstandarddirs.h>

#include <QFile>
#include <QDir>
#include <QTextStream>

namespace KCDDB
{
    CDInfoList
  Cache::lookup( const TrackOffsetList &offsetList, const Config& c )
  {
    QString cddbId = CDDB::trackOffsetListToId(offsetList);

    kDebug(60010) << "Looking up " << cddbId << " in CDDB cache" << endl;

    CDInfoList infoList;
    QStringList cddbCacheDirs = c.cacheLocations();

    for (QStringList::Iterator cddbCacheDir = cddbCacheDirs.begin();
        cddbCacheDir != cddbCacheDirs.end(); ++cddbCacheDir)
    {
      QDir dir( *cddbCacheDir );
      QStringList dirList = dir.entryList( QDir::Dirs );

      QStringList::ConstIterator it = dirList.begin();

      while ( it != dirList.end() )
      {
        QString category( *it );
        if ( category[ 0 ] != '.' )
        {
          QFile f( *cddbCacheDir + '/' + category + '/' + cddbId );
          if ( f.exists() && f.open(QIODevice::ReadOnly) )
          {
              QTextStream ts(&f);
              ts.setCodec("UTF-8");
              QString cddbData = ts.readAll();
              f.close();
              CDInfo info;
              info.load(cddbData);
              info.set(Category,category);

              infoList.append( info );
          }
        }
        ++it;
      }
    }

    return infoList;
  }

    void
  Cache::store(const TrackOffsetList& offsetList, const CDInfoList& list, const Config& c)
  {
    foreach( CDInfo info, list )
    {
      store(offsetList, info, c);
    }
  }

    void
  Cache::store(const TrackOffsetList& offsetList, const CDInfo& info, const Config& c)
  {
    QString discid = info.get("discid").toString();

    // Some entries from freedb could contain several discids separated
    // by a ','. Store for each discid, but replace the discid line
    // so it doesn't happen again.
    QStringList discids = discid.split(',');
    if (discids.count() > 2)
    {
      foreach(QString newid, discids)
      {
        CDInfo newInfo = info;
        newInfo.set("discid", newid);
        store(offsetList, newInfo, c);
      }
    }

    QString source = info.get("source").toString();

    QString cacheDir;
    QString cacheFile;

    CDInfo newInfo = info;

    if (source == "freedb")
    {
      cacheDir = '/' + info.get("category").toString() + '/';
      cacheFile = discid;
    }
    else if (source == "musicbrainz")
    {
      cacheDir = "/musicbrainz/";
      cacheFile = discid;
    }
    else if (source == "user")
    {
      cacheDir = "/user/";
      QString id = CDDB::trackOffsetListToId(offsetList);
      cacheFile = id;
      newInfo.set("discid", id);
    }
    else
    {
      kWarning(60010) << "Unknown source " << source << " for CDInfo, can't store discinfo" << endl;
      return;
    }

    cacheDir = c.cacheLocations().first() + cacheDir;

    QDir dir;

    if (!dir.exists(cacheDir))
    {
      if (!dir.mkpath(cacheDir))
      {
        kWarning(60010) << "Couldn't create cache directory " << cacheDir << endl;
        return;
      }
    }

    kDebug(60010) << "Storing " << cacheFile << " in CDDB cache" << endl;

    QFile f(cacheDir + '/' + cacheFile);
    if ( f.open(QIODevice::WriteOnly) )
    {
      QTextStream ts(&f);
      ts.setCodec("UTF-8");
      ts << newInfo.toString();
      f.close();
    }
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

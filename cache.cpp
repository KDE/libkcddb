/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002 Benjamin Meyer <ben-devel@meyerhome.net>
  Copyright (C) 2002 Nadeem Hasan <nhasan@kde.org>

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

#include <kdebug.h>
#include <kstandarddirs.h>

#include <qfile.h>
#include <qdir.h>
//Added by qt3to4:
#include <QTextStream>

#include "cache.h"
#include "config.h"

namespace KCDDB
{
    QString
  Cache::fileName( const CDInfo& info, const QString& cacheDir )
  {
    QDir dir( cacheDir );
    QString category(info.get(Category).toString());
    
    if ( !dir.exists( category ) )
      dir.mkdir( category );

    QString cacheFile = cacheDir + "/" + category + "/" + info.get("discid").toString();

    return cacheFile;
  }

    CDInfoList
  Cache::lookup( const QString &cddbId, const Config& c )
  {
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
          QFile f( *cddbCacheDir + "/" + category + "/" + cddbId );
          if ( f.exists() && f.open(QIODevice::ReadOnly) )
          {
              QTextStream ts(&f);
              ts.setEncoding(QTextStream::UnicodeUTF8);
              QString cddbData = ts.read();
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
  Cache::store(const CDInfoList& list, const Config& c)
  {
    CDInfoList::ConstIterator it=list.begin(); 
    while (it!=list.end())
    {
      CDInfo info( *it );
      store(info, c);
      ++it;
    }
  }

    void
  Cache::store(const CDInfo& info, const Config& c)
  {
    QString cacheDir = c.cacheLocations().first();
    QDir d(cacheDir);
    if (!d.exists())
      d.mkdir(cacheDir);

    QString cacheFile = fileName(info, cacheDir);

    kDebug(60010) << "Storing " << cacheFile << " in CDDB cache" << endl;

    QFile f(cacheFile);
    if ( f.open(QIODevice::WriteOnly) )
    {
      QTextStream ts(&f);
      ts.setEncoding(QTextStream::UnicodeUTF8);
      ts << info.toString();
      f.close();
    }
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

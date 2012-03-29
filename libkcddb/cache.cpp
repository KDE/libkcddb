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

#include "config-musicbrainz.h"
#ifdef HAVE_MUSICBRAINZ4
#include "musicbrainz/musicbrainzlookup.h"
#endif

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

    kDebug(60010) << "Looking up " << cddbId << " in CDDB cache";

    CDInfoList infoList;

    infoList << CDDB::cacheFiles(offsetList, c);
#ifdef HAVE_MUSICBRAINZ4
    infoList << MusicBrainzLookup::cacheFiles(offsetList, c);
#endif

    return infoList;
  }

    void
  Cache::store(const TrackOffsetList& offsetList, const CDInfoList& list, const Config& c)
  {
    foreach( const CDInfo &info, list )
    {
      store(offsetList, info, c);
    }
  }

    void
  Cache::store(const TrackOffsetList& offsetList, const CDInfo& info, const Config& c)
  {
    QString discid = info.get(QLatin1String( "discid" )).toString();

    // Some entries from freedb could contain several discids separated
    // by a ','. Store for each discid, but replace the discid line
    // so it doesn't happen again.
    QStringList discids = discid.split(QLatin1Char( ',' ));
    if (discids.count() > 2)
    {
      foreach(const QString &newid, discids)
      {
        CDInfo newInfo = info;
        newInfo.set(QLatin1String( "discid" ), newid);
        store(offsetList, newInfo, c);
      }
    }

    QString source = info.get(QLatin1String( "source" )).toString();

    QString cacheDir;
    QString cacheFile;

    CDInfo newInfo = info;

    if (source == QLatin1String( "freedb" ))
    {
      cacheDir = QLatin1Char( '/' ) + info.get(QLatin1String( "category" )).toString() + QLatin1Char( '/' );
      cacheFile = discid;
    }
    else if (source == QLatin1String( "musicbrainz" ))
    {
      cacheDir = QLatin1String( "/musicbrainz/" );
      cacheFile = discid;
    }
    else
    {
      if (source != QLatin1String( "user" ))
        kWarning(60010) << "Unknown source " << source << " for CDInfo";

      cacheDir = QLatin1String( "/user/" );
      QString id = CDDB::trackOffsetListToId(offsetList);
      cacheFile = id;
      newInfo.set(QLatin1String( "discid" ), id);
    }

    cacheDir = c.cacheLocations().first() + cacheDir;

    QDir dir;

    if (!dir.exists(cacheDir))
    {
      if (!dir.mkpath(cacheDir))
      {
        kWarning(60010) << "Couldn't create cache directory " << cacheDir;
        return;
      }
    }

    kDebug(60010) << "Storing " << cacheFile << " in CDDB cache";

    QFile f(cacheDir + QLatin1Char( '/' ) + cacheFile);
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

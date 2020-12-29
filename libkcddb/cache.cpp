/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>
    SPDX-FileCopyrightText: 2002 Nadeem Hasan <nhasan@kde.org>
    SPDX-FileCopyrightText: 2007 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "cache.h"

#include "kcddbconfig.h"
#include "cddb.h"
#include "logging.h"

#include "config-musicbrainz.h"
#ifdef HAVE_MUSICBRAINZ5
#include "musicbrainz/musicbrainzlookup.h"
#endif

#include <QFile>
#include <QDir>
#include <QTextStream>

namespace KCDDB
{
    CDInfoList
  Cache::lookup( const TrackOffsetList &offsetList, const Config& c )
  {
    QString cddbId = CDDB::trackOffsetListToId(offsetList);

	qCDebug(LIBKCDDB) << "Looking up " << cddbId << " in CDDB cache";

    CDInfoList infoList;

    infoList << CDDB::cacheFiles(offsetList, c);
#ifdef HAVE_MUSICBRAINZ5
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
		qCWarning(LIBKCDDB) << "Unknown source " << source << " for CDInfo";

      cacheDir = QLatin1String( "/user/" );
      QString id = CDDB::trackOffsetListToId(offsetList);
      cacheFile = id;
      newInfo.set(QLatin1String( "discid" ), id);
    }

    const QStringList cacheLocations = c.cacheLocations();

    if (!cacheLocations.isEmpty()) {
      cacheDir = cacheLocations.first() + cacheDir;

      QDir dir;

      if (!dir.exists(cacheDir))
      {
        if (!dir.mkpath(cacheDir))
        {
		      qCWarning(LIBKCDDB) << "Couldn't create cache directory " << cacheDir;
          return;
        }
      }

	    qCDebug(LIBKCDDB) << "Storing " << cacheFile << " in CDDB cache";

      QFile f(cacheDir + QLatin1Char( '/' ) + cacheFile);
      if ( f.open(QIODevice::WriteOnly) )
      {
        QTextStream ts(&f);
        ts.setCodec("UTF-8");
        ts << newInfo.toString();
        f.close();
      }
    } else {
      qDebug() << "There's no cache dir defined, not storing it";
    }
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

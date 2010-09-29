/*
  Copyright (C) 2005-2007 Richard Lärkäng <nouseforaname@home.se>

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


#include "musicbrainzlookup.h"

#include <kdebug.h>
#include <kcodecs.h>
#include <qcryptographichash.h>
#include <cstdio>
#include <cstring>
#include <musicbrainz3/musicbrainz.h>
#include <musicbrainz3/query.h>
#include <musicbrainz3/release.h>
#include <musicbrainz3/filters.h>

namespace KCDDB
{
  MusicBrainzLookup::MusicBrainzLookup()
  {

  }

  MusicBrainzLookup::~MusicBrainzLookup()
  {

  }

  Result MusicBrainzLookup::lookup( const QString &, uint, const TrackOffsetList & trackOffsetList )
  {
    QString discId = calculateDiscId(trackOffsetList);

    kDebug() << "Should lookup " << discId;

    MusicBrainz::Query q;
    MusicBrainz::ReleaseResultList results;

    try {
        MusicBrainz::ReleaseFilter f = MusicBrainz::ReleaseFilter().discId(std::string(discId.toAscii()));
        results = q.getReleases(&f);
    }
		// FIXME Doesn't seem to get caught, why?
    // catch (MusicBrainz::WebServiceError &e)
    catch (...)
    {
        kDebug() << "Query failed"; //<< e.what();
        return UnknownError;
    }

    int relnr=1;
    for (MusicBrainz::ReleaseResultList::iterator i = results.begin(); i != results.end(); i++) {
        MusicBrainz::ReleaseResult *result = *i;
        MusicBrainz::Release *release;
        try {
            release = q.getReleaseById(result->getRelease()->getId(), &MusicBrainz::ReleaseIncludes().tracks().artist());
        }
        catch (MusicBrainz::WebServiceError &e) {
            kDebug() << "Error: " << e.what();
            continue;
        }
        CDInfo info;
        info.set(QLatin1String( "source" ), QLatin1String( "musicbrainz" ));
        // Uses musicbrainz discid for the first release,
        // then discid-2, discid-3 and so on, to
        // allow multiple releases with the same discid
        if (relnr == 1)
          info.set(QLatin1String( "discid" ), discId);
        else
          info.set(QLatin1String( "discid" ), discId+QLatin1String( "-" )+QString::number(relnr));

        info.set(Title, QString::fromUtf8(release->getTitle().c_str()));
        info.set(Artist, QString::fromUtf8(release->getArtist()->getName().c_str()));

        int trackno = 0;
        for (MusicBrainz::TrackList::iterator j = release->getTracks().begin(); j != release->getTracks().end(); j++) {
            MusicBrainz::Track *mb_track = *j;
            MusicBrainz::Artist *artist = mb_track->getArtist();
            if (!artist)
              artist = release->getArtist();
            TrackInfo& track = info.track(trackno);
            track.set(Artist, QString::fromUtf8(artist->getName().c_str()));
            track.set(Title, QString::fromUtf8(mb_track->getTitle().c_str()));
            trackno++;
        }
        delete result;

        cdInfoList_ << info;
        relnr++;
    }

    if (cdInfoList_.isEmpty())
    {
        kDebug() << "No record found";
        return NoRecordFound;
    }

    kDebug() << "Query succeeded :-)";

    return Success;
  }

  QString MusicBrainzLookup::calculateDiscId(const TrackOffsetList & trackOffsetList )
  {
    // Code based on libmusicbrainz/lib/diskid.cpp

    int numTracks = trackOffsetList.count()-1;

    QCryptographicHash sha(QCryptographicHash::Sha1);
    char           temp[9];

    // FIXME How do I check that?
    int firstTrack = 1;
    int lastTrack = numTracks;

    sprintf(temp, "%02X", firstTrack);
    sha.addData(temp, strlen(temp));

    sprintf(temp, "%02X", lastTrack);
    sha.addData(temp, strlen(temp));

    for(int i = 0; i < 100; i++)
    {
      long offset;
      if (i == 0)
        offset = trackOffsetList[numTracks];
      else if (i <= numTracks)
        offset = trackOffsetList[i-1];
      else
        offset = 0;

      sprintf(temp, "%08lX", offset);
      sha.addData(temp, strlen(temp));
    }

    QByteArray base64 = sha.result().toBase64();

    // '/' '+' and '=' replaced for MusicBrainz
    QString res = QString::fromLatin1(base64).replace(QLatin1Char( '/' ),QLatin1String( "_" )).replace(QLatin1Char( '+' ),QLatin1String( "." )).replace(QLatin1Char( '=' ),QLatin1String( "-" ));

    return res;
  }

  CDInfoList MusicBrainzLookup::cacheFiles(const TrackOffsetList &offsetList, const Config& c )
  {
    CDInfoList infoList;
    QStringList cddbCacheDirs = c.cacheLocations();
    QString discid = calculateDiscId(offsetList);

    for (QStringList::const_iterator cddbCacheDir = cddbCacheDirs.constBegin();
        cddbCacheDir != cddbCacheDirs.constEnd(); ++cddbCacheDir)
    {
      // Looks for all files in cddbdir/musicbrainz/discid*
      // Several files can correspond to the same discid,
      // then they are named discid, discid-2, discid-3 and so on
      QDir dir(*cddbCacheDir+QLatin1String( "/musicbrainz/" ));
      dir.setNameFilters(QStringList(discid+QLatin1String( "*" )));

      QStringList files = dir.entryList();
      kDebug() << "Cache files found: " << files.count();
      for (QStringList::iterator it = files.begin(); it != files.end(); ++it)
      {
        QFile f( dir.filePath(*it) );
        if ( f.exists() && f.open(QIODevice::ReadOnly) )
        {
          QTextStream ts(&f);
          ts.setCodec("UTF-8");
          QString cddbData = ts.readAll();
          f.close();
          CDInfo info;
          info.load(cddbData);
          info.set(QLatin1String( "source" ), QLatin1String( "musicbrainz" ));
          info.set(QLatin1String( "discid" ), discid);

          infoList.append( info );
        }
        else
          kDebug() << "Could not read file: " << f.fileName();
      }
    }

    return infoList;
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

#include "musicbrainzlookup.moc"

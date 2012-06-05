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
#include <klocale.h>
#include <qcryptographichash.h>
#include <cstdio>
#include <cstring>
#include <musicbrainz4/Query.h>
#include "musicbrainz4/Medium.h"
#include "musicbrainz4/Release.h"
#include "musicbrainz4/ReleaseGroup.h"
#include "musicbrainz4/Track.h"
#include "musicbrainz4/Recording.h"
#include "musicbrainz4/Disc.h"
#include "musicbrainz4/HTTPFetch.h"
#include "musicbrainz4/ArtistCredit.h"
#include "musicbrainz4/Artist.h"
#include "musicbrainz4/NameCredit.h"

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

    MusicBrainz4::CQuery Query("libkcddb-0.5");

    // Code adapted from libmusicbrainz/examples/cdlookup.cc

    try {
      MusicBrainz4::CMetadata Metadata=Query.Query("discid",discId.toAscii().constData());

      if (Metadata.Disc() && Metadata.Disc()->ReleaseList())
      {
        MusicBrainz4::CReleaseList *ReleaseList=Metadata.Disc()->ReleaseList();
        kDebug() << "Found " << ReleaseList->NumItems() << " release(s)";

        int relnr=1;

        for (int i = 0; i < ReleaseList->NumItems(); i++)
        {
          MusicBrainz4::CRelease* Release=ReleaseList->Item(i);

          //The releases returned from LookupDiscID don't contain full information

          MusicBrainz4::CQuery::tParamMap Params;
          Params["inc"]="artists labels recordings release-groups url-rels discids artist-credits";

          std::string ReleaseID=Release->ID();

          MusicBrainz4::CMetadata Metadata2=Query.Query("release",ReleaseID,"",Params);
          if (Metadata2.Release())
          {
            MusicBrainz4::CRelease *FullRelease=Metadata2.Release();

            //However, these releases will include information for all media in the release
            //So we need to filter out the only the media we want.

            MusicBrainz4::CMediumList MediaList=FullRelease->MediaMatchingDiscID(discId.toAscii().constData());

            if (MediaList.NumItems() > 0)
            {
              /*if (FullRelease->ReleaseGroup())
                kDebug() << "Release group title: " << FullRelease->ReleaseGroup()->Title();
              else
                kDebug() << "No release group for this release";*/

              kDebug() << "Found " << MediaList.NumItems() << " media item(s)";

              for (int i=0; i < MediaList.NumItems(); i++)
              {
                MusicBrainz4::CMedium* Medium= MediaList.Item(i);

                /*kDebug() << "Found media: '" << Medium.Title() << "', position " << Medium.Position();*/

                CDInfo info;
                info.set(QLatin1String( "source" ), QLatin1String( "musicbrainz" ));
                // Uses musicbrainz discid for the first release,
                // then discid-2, discid-3 and so on, to
                // allow multiple releases with the same discid
                if (relnr == 1)
                  info.set(QLatin1String( "discid" ), discId);
                else
                  info.set(QLatin1String( "discid" ), discId+QLatin1String( "-" )+QString::number(relnr));

                QString title = QString::fromUtf8(FullRelease->Title().c_str());

                if (FullRelease->MediumList()->NumItems() > 1)
                  title = i18n("%1 (disc %2)", title, Medium->Position());

                info.set(Title, title);
                info.set(Artist, artistFromCreditList(FullRelease->ArtistCredit()));

                MusicBrainz4::CTrackList *TrackList=Medium->TrackList();
                if (TrackList)
                {
                  for (int i=0; i < TrackList->NumItems(); i++)
                  {
                    MusicBrainz4::CTrack* Track=TrackList->Item(i);
                    MusicBrainz4::CRecording *Recording=Track->Recording();

                    /*if (Recording)
                      kDebug() << "Track: " << Track.Position() << " - '" << Recording->Title() << "'";
                    else
                      kDebug() << "Track: " << Track.Position() << " - '" << Track.Title() << "'";*/
                    TrackInfo& track = info.track(i);
                    if (Recording)
                    {
                      track.set(Artist, artistFromCreditList(Recording->ArtistCredit()));
                      track.set(Title, QString::fromUtf8(Recording->Title().c_str()));
                    }
                    else
                    {
                      track.set(Artist, artistFromCreditList(Track->ArtistCredit()));
                      track.set(Title, QString::fromUtf8(Track->Title().c_str()));
                    }
                  }
                }
                cdInfoList_ << info;
                relnr++;
              }
            }
          }
        }
      }
    }

    catch (MusicBrainz4::CConnectionError& Error)
    {
      kDebug() << "Connection Exception: '" << Error.what() << "'";
      kDebug() << "LastResult: " << Query.LastResult();
      kDebug() << "LastHTTPCode: " << Query.LastHTTPCode();
      kDebug() << "LastErrorMessage: " << QString::fromUtf8(Query.LastErrorMessage().c_str());

      return ServerError;
    }

    catch (MusicBrainz4::CTimeoutError& Error)
    {
      kDebug() << "Timeout Exception: '" << Error.what() << "'";
      kDebug() << "LastResult: " << Query.LastResult();
      kDebug() << "LastHTTPCode: " << Query.LastHTTPCode();
      kDebug() << "LastErrorMessage: " << QString::fromUtf8(Query.LastErrorMessage().c_str());

      return ServerError;
    }

    catch (MusicBrainz4::CAuthenticationError& Error)
    {
      kDebug() << "Authentication Exception: '" << Error.what() << "'";
      kDebug() << "LastResult: " << Query.LastResult();
      kDebug() << "LastHTTPCode: " << Query.LastHTTPCode();
      kDebug() << "LastErrorMessage: " << QString::fromUtf8(Query.LastErrorMessage().c_str());

      return ServerError;
    }

    catch (MusicBrainz4::CFetchError& Error)
    {
      kDebug() << "Fetch Exception: '" << Error.what() << "'";
      kDebug() << "LastResult: " << Query.LastResult();
      kDebug() << "LastHTTPCode: " << Query.LastHTTPCode();
      kDebug() << "LastErrorMessage: " << QString::fromUtf8(Query.LastErrorMessage().c_str());

      return ServerError;
    }

    catch (MusicBrainz4::CRequestError& Error)
    {
      kDebug() << "Request Exception: '" << Error.what() << "'";
      kDebug() << "LastResult: " << Query.LastResult();
      kDebug() << "LastHTTPCode: " << Query.LastHTTPCode();
      kDebug() << "LastErrorMessage: " << QString::fromUtf8(Query.LastErrorMessage().c_str());

      return ServerError;
    }

    catch (MusicBrainz4::CResourceNotFoundError& Error)
    {
      kDebug() << "ResourceNotFound Exception: '" << Error.what() << "'";
      kDebug() << "LastResult: " << Query.LastResult();
      kDebug() << "LastHTTPCode: " << Query.LastHTTPCode();
      kDebug() << "LastErrorMessage: " << QString::fromUtf8(Query.LastErrorMessage().c_str());

      return ServerError;
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

  QString MusicBrainzLookup::artistFromCreditList(MusicBrainz4::CArtistCredit * artistCredit )
  {
    kDebug() << k_funcinfo;
    QString artistName;

    MusicBrainz4::CNameCreditList *ArtistList=artistCredit->NameCreditList();

    if (ArtistList)
    {
      for (int i=0; i < ArtistList->NumItems(); i++)
      {
        MusicBrainz4::CNameCredit* Name=ArtistList->Item(i);
        MusicBrainz4::CArtist* Artist = Name->Artist();

        if (!Name->Name().empty())
          artistName += QString::fromUtf8(Name->Name().c_str());
        else
          artistName += QString::fromUtf8(Artist->Name().c_str());

        artistName += QString::fromUtf8(Name->JoinPhrase().c_str());
      }

      kDebug() << "Artist:" << artistName;

    }

    return artistName;
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

#include "musicbrainzlookup.moc"

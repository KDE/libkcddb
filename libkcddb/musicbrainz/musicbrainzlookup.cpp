/*
  Copyright (C) 2005-2007 Richard Lärkäng <nouseforaname@home.se>
  Copyright (C) 2016 Angelo Scarnà <angelo.scarna@codelinsoft.it>

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

#include <QDebug>
#include <KCodecs/KCodecs>
#include <KLocalizedString>
#include <qcryptographichash.h>
#include <cstdio>
#include <cstring>
#include <musicbrainz5/Query.h>
#include <musicbrainz5/Medium.h>
#include <musicbrainz5/Release.h>
#include <musicbrainz5/ReleaseGroup.h>
#include <musicbrainz5/Track.h>
#include <musicbrainz5/Recording.h>
#include <musicbrainz5/Disc.h>
#include <musicbrainz5/HTTPFetch.h>
#include <musicbrainz5/ArtistCredit.h>
#include <musicbrainz5/Artist.h>
#include <musicbrainz5/NameCredit.h>
#include <musicbrainz5/SecondaryType.h>

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

    qDebug() << "Should lookup " << discId;

    MusicBrainz5::CQuery Query("libkcddb-0.5");

    // Code adapted from libmusicbrainz/examples/cdlookup.cc

    try {
      MusicBrainz5::CMetadata Metadata=Query.Query("discid",discId.toLatin1().constData());

      if (Metadata.Disc() && Metadata.Disc()->ReleaseList())
      {
        MusicBrainz5::CReleaseList *ReleaseList=Metadata.Disc()->ReleaseList();
        qDebug() << "Found " << ReleaseList->NumItems() << " release(s)";

        int relnr=1;

        for (int i = 0; i < ReleaseList->NumItems(); i++)
        {
          MusicBrainz5::CRelease* Release=ReleaseList->Item(i);

          //The releases returned from LookupDiscID don't contain full information

          MusicBrainz5::CQuery::tParamMap Params;
          Params["inc"]="artists labels recordings release-groups url-rels discids artist-credits";

          std::string ReleaseID=Release->ID();

          MusicBrainz5::CMetadata Metadata2=Query.Query("release",ReleaseID,"",Params);
          if (Metadata2.Release())
          {
            MusicBrainz5::CRelease *FullRelease=Metadata2.Release();

            //However, these releases will include information for all media in the release
            //So we need to filter out the only the media we want.

            MusicBrainz5::CMediumList MediaList=FullRelease->MediaMatchingDiscID(discId.toLatin1().constData());

            if (MediaList.NumItems() > 0)
            {
              /*if (FullRelease->ReleaseGroup())
                qDebug() << "Release group title: " << FullRelease->ReleaseGroup()->Title();
              else
                qDebug() << "No release group for this release";*/

              qDebug() << "Found " << MediaList.NumItems() << " media item(s)";

              for (int i=0; i < MediaList.NumItems(); i++)
              {
                MusicBrainz5::CMedium* Medium= MediaList.Item(i);

                /*qDebug() << "Found media: '" << Medium.Title() << "', position " << Medium.Position();*/

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

                QString date = QString::fromUtf8(FullRelease->Date().c_str());
                QRegExp yearRe("^(\\d{4,4})(-\\d{1,2}-\\d{1,2})?$");
                int year = 0;
                if (yearRe.indexIn(date) > -1)
                {
                  QString yearString = yearRe.cap(1);
                  bool ok;
                  year=yearString.toInt(&ok);
                  if (!ok)
                    year = 0;
                }
                info.set(Year, year);

                MusicBrainz5::CTrackList *TrackList=Medium->TrackList();
                if (TrackList)
                {
                  for (int i=0; i < TrackList->NumItems(); i++)
                  {
                    MusicBrainz5::CTrack* Track=TrackList->Item(i);
                    MusicBrainz5::CRecording *Recording=Track->Recording();

                    TrackInfo& track = info.track(i);

                    // Prefer title and artist from the track credits, but
                    // it appears to be empty if same as in Recording
                    // Noticable in the musicbrainztest-fulldate test,
                    // where the title on the credits of track 18 are
                    // "Bara om min älskade väntar", but the recording
                    // has title "Men bara om min älskade"
                    if(Recording && Track->ArtistCredit() == 0)
                      track.set(Artist, artistFromCreditList(Recording->ArtistCredit()));
                    else
                      track.set(Artist, artistFromCreditList(Track->ArtistCredit()));

                    if(Recording && Track->Title().empty())
                      track.set(Title, QString::fromUtf8(Recording->Title().c_str()));
                    else
                      track.set(Title, QString::fromUtf8(Track->Title().c_str()));
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

    catch (MusicBrainz5::CConnectionError& Error)
    {
      qDebug() << "Connection Exception: '" << Error.what() << "'";
      qDebug() << "LastResult: " << Query.LastResult();
      qDebug() << "LastHTTPCode: " << Query.LastHTTPCode();
      qDebug() << "LastErrorMessage: " << QString::fromUtf8(Query.LastErrorMessage().c_str());

      return ServerError;
    }

    catch (MusicBrainz5::CTimeoutError& Error)
    {
      qDebug() << "Timeout Exception: '" << Error.what() << "'";
      qDebug() << "LastResult: " << Query.LastResult();
      qDebug() << "LastHTTPCode: " << Query.LastHTTPCode();
      qDebug() << "LastErrorMessage: " << QString::fromUtf8(Query.LastErrorMessage().c_str());

      return ServerError;
    }

    catch (MusicBrainz5::CAuthenticationError& Error)
    {
      qDebug() << "Authentication Exception: '" << Error.what() << "'";
      qDebug() << "LastResult: " << Query.LastResult();
      qDebug() << "LastHTTPCode: " << Query.LastHTTPCode();
      qDebug() << "LastErrorMessage: " << QString::fromUtf8(Query.LastErrorMessage().c_str());

      return ServerError;
    }

    catch (MusicBrainz5::CFetchError& Error)
    {
      qDebug() << "Fetch Exception: '" << Error.what() << "'";
      qDebug() << "LastResult: " << Query.LastResult();
      qDebug() << "LastHTTPCode: " << Query.LastHTTPCode();
      qDebug() << "LastErrorMessage: " << QString::fromUtf8(Query.LastErrorMessage().c_str());

      return ServerError;
    }

    catch (MusicBrainz5::CRequestError& Error)
    {
      qDebug() << "Request Exception: '" << Error.what() << "'";
      qDebug() << "LastResult: " << Query.LastResult();
      qDebug() << "LastHTTPCode: " << Query.LastHTTPCode();
      qDebug() << "LastErrorMessage: " << QString::fromUtf8(Query.LastErrorMessage().c_str());

      return ServerError;
    }

    catch (MusicBrainz5::CResourceNotFoundError& Error)
    {
      qDebug() << "ResourceNotFound Exception: '" << Error.what() << "'";
      qDebug() << "LastResult: " << Query.LastResult();
      qDebug() << "LastHTTPCode: " << Query.LastHTTPCode();
      qDebug() << "LastErrorMessage: " << QString::fromUtf8(Query.LastErrorMessage().c_str());

      return ServerError;
    }

    if (cdInfoList_.isEmpty())
    {
        qDebug() << "No record found";
        return NoRecordFound;
    }

    qDebug() << "Query succeeded :-)";

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
      qDebug() << "Cache files found: " << files.count();
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
          qDebug() << "Could not read file: " << f.fileName();
      }
    }

    return infoList;
  }

  QString MusicBrainzLookup::artistFromCreditList(MusicBrainz5::CArtistCredit * artistCredit )
  {
    QString artistName;

    MusicBrainz5::CNameCreditList *ArtistList=artistCredit->NameCreditList();

    if (ArtistList)
    {
      for (int i=0; i < ArtistList->NumItems(); i++)
      {
        MusicBrainz5::CNameCredit* Name=ArtistList->Item(i);
        MusicBrainz5::CArtist* Artist = Name->Artist();

        if (!Name->Name().empty())
          artistName += QString::fromUtf8(Name->Name().c_str());
        else
          artistName += QString::fromUtf8(Artist->Name().c_str());

        artistName += QString::fromUtf8(Name->JoinPhrase().c_str());
      }

      qDebug() << "Artist:" << artistName;

    }

    return artistName;
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

#include "musicbrainzlookup.moc"

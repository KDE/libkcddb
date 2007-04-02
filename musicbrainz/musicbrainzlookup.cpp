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
#include <musicbrainz/musicbrainz.h>

// Added in libmusicbrainz 2.1.3
#ifndef MBE_AlbumGetAlbumArtistName
#define MBE_AlbumGetAlbumArtistName \
  "http://purl.org/dc/elements/1.1/creator http://purl.org/dc/elements/1.1/title"
#endif

namespace KCDDB
{
  MusicBrainzLookup::MusicBrainzLookup()
  {

  }

  MusicBrainzLookup::~MusicBrainzLookup()
  {

  }
  
  CDDB::Result MusicBrainzLookup::lookup( const QString &, uint, const TrackOffsetList & trackOffsetList )
  {
    QString discId = calculateDiscId(trackOffsetList);
    
    kDebug() << "Should lookup " << discId << endl;

    ::MusicBrainz mb;

    mb.UseUTF8(true);
    mb.SetDepth(4);

    vector<string> args;
    args.insert(args.begin(), discId.toLatin1().data());

    if (!mb.Query(MBQ_GetCDInfoFromCDIndexId, &args))
    {
      string error;
      
      mb.GetQueryError(error);
      kDebug() << "Query failed: " << error.c_str() << endl;
      
      return UnknownError;
    }

    int nrAlbums = mb.DataInt(MBE_GetNumAlbums);

    if (nrAlbums < 1)
    {
      kDebug() << "No CD Found" << endl;

      return UnknownError;
    }

    for (int i=1; i <= nrAlbums; i++)
    {
      mb.Select(MBS_SelectAlbum, i);

      CDInfo info;

      info.set(Title, QString::fromUtf8(mb.Data(MBE_AlbumGetAlbumName).c_str()));
      info.set(Artist, QString::fromUtf8(mb.Data(MBE_AlbumGetAlbumArtistName).c_str()));

      int numTracks = trackOffsetList.count()-1;

      for (int i=1; i <= numTracks; i++)
      {
        TrackInfo& track = info.track(i-1);
        track.set(Artist, QString::fromUtf8(mb.Data(MBE_AlbumGetArtistName, i).c_str()));
        track.set(Title, QString::fromUtf8(mb.Data(MBE_AlbumGetTrackName, i).c_str()));
      }

      cdInfoList_ << info;
    }

    kDebug() << "Query succeeded :-)" << endl;

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
    
    QByteArray base64 = KCodecs::base64Encode(sha.result());

    // '/' '+' and '=' replaced for MusicBrainz
    QString res = QString::fromLatin1(base64).replace('/',"_").replace('+',".").replace('=',"-");
    
    return res;
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

#include "musicbrainzlookup.moc"

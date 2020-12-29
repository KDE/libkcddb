/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>
    SPDX-FileCopyrightText: 2003-2005 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "submit.h"
#include "logging.h"

namespace KCDDB
{
  Submit::Submit()
    : CDDB()
  {
    // Empty.
  }

  Submit::~Submit()
  {
    // Empty.
  }

  Result Submit::submit( CDInfo cdInfo, const TrackOffsetList& offsetList)
  {
    // If it was an inexact math from the server the discid might
    // be different, so recalculate it
    cdInfo.set(QLatin1String( "discid" ), trackOffsetListToId(offsetList));

    makeDiskData( cdInfo, offsetList );

    if (!validCategory(cdInfo.get(Category).toString()))
      return InvalidCategory;

    KIO::Job* job = createJob(cdInfo);

    if (!job)
      return UnknownError;

    return runJob(job);
  }

    Result
  Submit::parseWrite(  const QString & line )
  {
    uint serverStatus = statusCode(  line );

    if (   320 != serverStatus )
      return ServerError;

    return Success;
  }

  void Submit::makeDiskData( const CDInfo& cdInfo, const TrackOffsetList& offsetList )
  {
    unsigned numTracks = cdInfo.numberOfTracks();

    diskData_ += QLatin1String( "# xmcd\n" );
    diskData_ += QLatin1String( "#\n" );
    diskData_ += QLatin1String( "# Track frame offsets:\n" );

    for (uint i=0; i < numTracks; i++)
        diskData_ += QString::fromLatin1("#\t%1\n").arg(offsetList[i]);

    int l = offsetList[numTracks]/75;
    diskData_ += QString::fromLatin1("# Disc length: %1 seconds\n").arg(l);

    diskData_ += cdInfo.toString(true);

	qCDebug(LIBKCDDB) << "diskData_ == " << diskData_;
  }

  bool Submit::validCategory( const QString& c )
  {
    QStringList validCategories;
    validCategories << QLatin1String( "blues" ) << QLatin1String( "classical" ) << QLatin1String( "country" )
      << QLatin1String( "data" ) << QLatin1String( "folk" ) << QLatin1String( "jazz" ) << QLatin1String( "misc" ) << QLatin1String( "newage" ) << QLatin1String( "reggae" )
      << QLatin1String( "rock" ) << QLatin1String( "soundtrack" );

    if (validCategories.contains(c))
      return true;
    else
      return false;
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

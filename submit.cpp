/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002 Benjamin Meyer <ben-devel@meyerhome.net>
  Copyright (C) 2003-2004 Richard Lärkäng <nouseforaname@home.se>

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
  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA 02111-1307, USA.
*/

#include "submit.h"
#include <kdebug.h>

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

    CDDB::Result
  Submit::parseWrite(  const QString & line )
  {
    uint serverStatus = statusCode(  line );

    if (   320 != serverStatus )
      return ServerError;

    return Success;
  }

  void Submit::makeDiskData( const CDInfo& cdInfo, const TrackOffsetList& offsetList )
  {
    unsigned numTracks = cdInfo.trackInfoList.count();

    diskData_ = "\n";
    diskData_ += "# xmcd\n";
    diskData_ += "#\n";
    diskData_ += "# Track frame offsets:\n";

    for (uint i=0; i < numTracks; i++)
      diskData_ += QString("#\t%1\n").arg(offsetList[i]);

    unsigned int l;
    if (cdInfo.length == 0)
    {
      l = (offsetList[numTracks+1] - offsetList[0]) / 75;
      // FIXME Is the submit test wrong, or the disc id calculation?
      l += 2;
    }
    else
      l = cdInfo.length;

    diskData_ += QString("# Disc length: %1 seconds\n").arg(l);

    diskData_ += cdInfo.toString(true);

    kdDebug(60010) << "diskData_ == " << diskData_ << endl;
  }

  bool Submit::validCategory( const QString& c )
  {
    QStringList validCategories;
    validCategories << "blues" << "classical" << "country"
      << "data" << "folk" << "jazz" << "misc" << "newage" << "reggae"
      << "rock" << "soundtrack";

    if (validCategories.contains(c))
      return true;
    else
      return false;
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

/*
  Copyright (C) 2003 Richard Lärkäng <nouseforaname@home.se>

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

#include "synchttpsubmit.h"
#include <kdebug.h>
#include <kio/netaccess.h>
#include <kio/job.h>

namespace KCDDB
{
  SyncHTTPSubmit::SyncHTTPSubmit()
    : HTTPSubmit()
  {

  }

  SyncHTTPSubmit::~SyncHTTPSubmit()
  {

  }
  
  CDDB::Result SyncHTTPSubmit::submit( const CDInfo &cdInfo, const TrackOffsetList& offsetList )
  {
    if (!validCategory(cdInfo.category))
      return InvalidCategory;

    KURL url("http://freedb.freedb.org/~cddb/submit.cgi");

    QString diskData;

    diskData = "# xmcd\n";
    diskData += "\n";
    diskData += "# Track frame offsets:\n";

    unsigned numTracks = cdInfo.trackInfoList.count();

    for (uint i=0; i < numTracks; i++)
      diskData += QString("#\t%1\n").arg(offsetList[i]);

    unsigned int l;
    if (cdInfo.length == 0)
    {
      l = (offsetList[numTracks+1] - offsetList[0]) / 75;
      // FIXME Is the submit test wrong, or the disc id calculation?
      l += 2;
    }
    else
      l = cdInfo.length;

    diskData += QString("# Disc length: %1 seconds\n").arg(l);

    diskData += cdInfo.toString(true) + "\n";

    kdDebug() << "diskData == \"" << diskData << "\"" << endl;

    KIO::TransferJob* job = KIO::http_post(url, diskData.utf8(), false);
    job->addMetaData("content-type", "Content-Type: text/plain");
    QString header;

    header += "Content-Type: text/plain\n";

    header += "Category: " + cdInfo.category + "\n";
    header += "Discid: " + cdInfo.id + "\n";
    header += "User-Email: nouseforaname@home.se\n";
    header += "Submit-Mode: test\n"; // s/test/submit/
    header += "Charset: UTF-8";

    job->addMetaData("customHTTPHeader", header);

    bool success = KIO::NetAccess::synchronousRun(job, 0, false);

    if (success)
      return CDDB::Success;
    else
      return CDDB::UnknownError;
  }
}

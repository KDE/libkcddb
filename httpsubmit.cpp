/*
  Copyright (C) 2002 Benjamin Meyer <ben-devel@meyerhome.net>
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
#include "httpsubmit.h"
#include <kurl.h>
#include <kdebug.h>
#include <kio/job.h>

namespace KCDDB
{
  HTTPSubmit::HTTPSubmit()
    : Submit()
  {

  }

  HTTPSubmit::~HTTPSubmit()
  {

  }

  CDDB::Result HTTPSubmit::submit(const CDInfo& cdInfo, const TrackOffsetList& offsetList)
  {
    if (!validCategory(cdInfo.category))
      return InvalidCategory;

    KURL url("http://freedb.freedb.org/~cddb/submit.cgi");

    makeDiskData(cdInfo, offsetList);

    KIO::TransferJob* job = KIO::http_post(url, diskData_.utf8(), false);
    if (!job)
      return UnknownError;

    job->addMetaData("content-type", "Content-Type: text/plain");
    QString header;

    header += "Content-Type: text/plain\n";

    header += "Category: " + cdInfo.category + "\n";
    header += "Discid: " + cdInfo.id + "\n";
    header += "User-Email: nouseforaname@home.se\n";
    header += "Submit-Mode: test\n"; // s/test/submit/
    header += "Charset: UTF-8";

    job->addMetaData("customHTTPHeader", header);

    return postData(job);
  }
}

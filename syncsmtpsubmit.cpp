/*
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

#include "syncsmtpsubmit.h"
#include "cdinfo.h"
#include <kio/netaccess.h>
#include <kio/job.h>

namespace KCDDB
{
  SyncSMTPSubmit::SyncSMTPSubmit(const QString& hostname, uint port,
        const QString& username, const QString& from, const QString& to)
    : SMTPSubmit( hostname, port, username, from, to )
  {

  }

  SyncSMTPSubmit::~SyncSMTPSubmit()
  {

  }

  CDDB::Result SyncSMTPSubmit::submit( const CDInfo& cdInfo, const TrackOffsetList& offsetList )
  {
    makeDiskData( cdInfo, offsetList );

    if (!validCategory(cdInfo.category))
      return InvalidCategory;

    QString subject = QString("cddb %1 %2").arg(cdInfo.category, cdInfo.id);
    makeURL(subject);

    KIO::Job* job = KIO::storedPut(diskData_.utf8(), url_, -1, false, false, false);

    if ( KIO::NetAccess::synchronousRun(job, 0) )
      return Success;

    return UnknownError;
  }
}

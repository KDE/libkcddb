/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002 Benjamin Meyer <ben-devel@meyerhome.net>

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

#include <qstringlist.h>
#include <qdir.h>
#include <qfile.h>
#include <kextendedsocket.h>
#include <kdebug.h>
#include <qsocket.h>

#include "client.h"
//#include "synccddblookup.h"
#include "cache.h"

namespace KCDDB
{
  class Client::Private
  {
    public:

      Config config;

      CDInfoList cdInfoList;

      QString            cddbId;

      KExtendedSocket    socket;
  };

  Client::Client()
  {
    d = new Private;

    d->config.load();
  }

  Client::Client(const Config & config)
  {
    d = new Private;

    d->config = config;
  }

  Client::~Client()
  {
    delete d;
  }

    Config
  Client::config() const
  {
    return d->config;
  }

    CDInfoList
  Client::lookupResponse() const
  {
    return d->cdInfoList;
  }

    Lookup::Result
  Client::lookup(const TrackOffsetList & trackOffsetList)
  {
/*
    // Get the cddb id from trackOffsetList.

    d->cddbId = trackOffsetListToId(trackOffsetList);
    kdDebug() << k_funcinfo << "cddbId: " << d->cddbId << endl;

    if (!d->cddbId)
    {
      kdDebug() << "Can't create cddbid from offset list" << endl;
      return Lookup::NoRecordFound;
    }

    if (Cache::Ignore != d->config.cachePolicy())
    {
      d->cdInfoList = Cache::lookup(trackOffsetList);

      if (!d->cdInfoList.isEmpty())
        return Lookup::Success;
    }

    // If we're only supposed to try the cache and we failed, drop out now.

    if (Cache::Only == d->config.cachePolicy())
    {
      kdDebug() << "Only trying cache. Give up now." << endl;
      return Lookup::NoRecordFound;
    }

    // Do the actual lookup.

    switch (d->config.lookupTransport())
    {
      case Lookup::CDDB:
        {
          SyncCDDBLookup lookup;

          Lookup::Result r =
            lookup.lookup
            (
              trackOffsetList,
              d->config.hostname(),
              d->config.port(),
              d->config.clientName(),
              d->config.clientVersion()
            );

          d->cdInfoList = lookup.lookupResponse();

          return r;
        }

        break;

      case Lookup::HTTP:

      default:
        kdDebug() << k_funcinfo << "Unsupported transport: "
          << Lookup::transportToString(d->config.lookupTransport()) << endl;
        return Lookup::UnknownError;
        break;
    }
*/
  }

    Submit::Result
  Client::submit(const CDInfo &)
  {
    return Submit::UnknownError;
#if 0
    // Do CannotSave sinarios
    if(cdInfo.id == "0")
      return CannotSave;
    for (uint i=0; i < cdInfo.trackInfoList.count(); i++)
      if(!cdInfo.trackInfoList[i].offsetKnown)
        return CannotSave;

QString diskData = "";
diskData += "# xmcd\n";
diskData += "#\n";
diskData += "# Track frame offsets:\n";
for (uint i=0; i < cdInfo.trackInfoList.count(); i++){
  diskData += QString("#\t%1\n").arg(cdInfo.trackInfoList[i].offset);
}
diskData += "#\n";
diskData += QString("DISCID=%1\n").arg(cdInfo.id);
diskData += QString("DTITLE=%1\n").arg(cdInfo.title);
diskData += QString("DYEAR=%1\n").arg(cdInfo.year);
diskData += QString("DGENRE=%1\n").arg(cdInfo.genre);
for (uint i=0; i < cdInfo.trackInfoList.count(); i++){
  diskData += QString("TTITLE%1=%2\n").arg(i).arg(cdInfo.trackInfoList[i].title);
}

    switch (d->config.submitTransport())
    {
      case HTTPSubmit:
      {

#define sendOut(a) submitSocket.writeBlock( a.latin1(), a.length() );

QSocket submitSocket(0, "http Sumbition Socket");
submitSocket.connectToHost("www.freecddb.org", 80);
sendOut(QString("POST /submit.cgi HTTP/1.0\n"));
sendOut(QString("Category: %1\n").arg(cdInfo.genre));
sendOut(QString("Discid: %1\n").arg(cdInfo.id));
sendOut(QString("User-Email: ben@meyerhome.net\n"));
sendOut(QString("Submit-Mode: test\n")); // Change to "submit"
sendOut(QString("Charset: ISO-8859-1\n"));
sendOut(QString("X-Cddbd-Note: Sent by libkcddb - Questions: ben@meyerhome.net.\n"));
sendOut(QString("Content-Length: %1\n").arg(diskData.length())); // Get real length()
sendOut(QString("\n"));
sendOut(diskData);

#undef sendOut
return None;

      }
      break;
      
      case SMTPSubmit:
      {
        return CannotSave;
      }
      break;

      default:
        kdDebug() << k_funcinfo << "Unsupported transport: "
          << submitTransportToString(d->config.submitTransport()) << endl;
        return Unknown;
        break;
    }
#endif
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

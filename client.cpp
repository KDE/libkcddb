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

#include <qdir.h>
#include <qfile.h>
#include <kextsock.h>
#include <kdebug.h>

#include <libkcddb/client.h>

namespace KCDDB
{
  class Client::Private
  {
    public:

      Config config;

      QValueList<CDInfo> cdInfoList;

      QString            cddbId;

      KExtendedSocket    socket;
  };

  static Error connectSocket
    (
      KExtendedSocket & socket,
      const QString   & hostname,
      uint              port
    )
  {
    socket.setHost(hostname);
    socket.setPort(port);

    int lookupReturn = socket.lookup();

    if (0 != lookupReturn)
      return HostNotFound;

    int connectReturn = socket.connect();

    if (0 != connectReturn)
      return NoResponse;

    return None;
  }

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

    QValueList<CDInfo>
  Client::lookupResponse() const
  {
    return d->cdInfoList;
  }

    Error
  Client::lookup(const TrackOffsetList & trackOffsetList)
  {
    // Get the cddb id from trackOffsetList.

    d->cddbId = trackOffsetListToId(trackOffsetList);
    kdDebug() << k_funcinfo << "cddbId: " << d->cddbId << endl;

    if (!d->cddbId)
    {
      kdDebug() << "Can't create cddbid from offset list" << endl;
      return NoSuchCD;
    }

    // Try the cache if we're supposed to.

    if (
      CDDBLookupIgnoreCached != d->config.lookupTransport()
      &&
      HTTPLookupIgnoreCached != d->config.lookupTransport()
    )
    {
      kdDebug() << "Should try cache here" << endl;

      QFile f(QDir::homeDirPath() + "/.cddb/" + d->cddbId);

      if (f.exists())
      {
        // STUB
        kdDebug() << "Found locally cached info" << endl;
        return Unknown;
      }

      kdDebug() << "Didn't find locally cached info" << endl;
    }

    // If we're only supposed to try the cache and we failed, drop out now.

    if (CacheOnlyLookup == d->config.lookupTransport())
    {
      kdDebug() << "Only trying cache. Give up now." << endl;
      return NoSuchCD;
    }

    // Do the actual lookup.

    switch (d->config.lookupTransport())
    {
      case CDDBLookup:
      case CDDBLookupIgnoreCached:
        {
          Error connectError =
            connectSocket(d->socket, d->config.hostname(), d->config.port());

          if (None != connectError)
            return connectError;

          // STUB
          return Unknown;
        }
        break;

      case HTTPLookup:
      case HTTPLookupIgnoreCached:
        {
          Error connectError =
            connectSocket(d->socket, d->config.hostname(), d->config.port());

          if (None != connectError)
            return connectError;

          // STUB
          return Unknown;
        }

      default:
        kdDebug() << k_funcinfo << "Unsupported transport: "
          << lookupTransportToString(d->config.lookupTransport()) << endl;
        return Unknown;
        break;
    }
  }

    Error
  Client::submit(const CDInfo &)
  {
    switch (d->config.submitTransport())
    {
      case CDDBSubmit:
        {
          Error connectError =
            connectSocket(d->socket, d->config.hostname(), d->config.port());

          if (None != connectError)
            return connectError;

          // STUB
          return Unknown;
        }
        break;

      default:
        kdDebug() << k_funcinfo << "Unsupported transport: "
          << submitTransportToString(d->config.submitTransport()) << endl;
        return Unknown;
        break;
    }
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

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

#include <kdebug.h>

#include <libkcddb/asyncclient.h>
#include <libkcddb/asynccddblookup.h>
//#include <libkcddb/asynchttplookup.h>

namespace KCDDB
{
  class AsyncClient::Private
  {
    public:

      QString               cddbId;
      Config                config;
      QValueList<CDInfo>    lookupResponse;
      QObject             * helper;
  };

  AsyncClient::AsyncClient(QObject * parent, const char * name)
    : QObject(parent, name)
  {
    d = new Private;
    d->helper = 0;
  }

  AsyncClient::AsyncClient
    (
      const Config  & config,
      QObject       * parent,
      const char    * name
    )
    : QObject(parent, name)
  {
    d = new Private;
    d->config = config;
    d->helper = 0;
  }

  AsyncClient::~AsyncClient()
  {
    delete d;
  }

    Config
  AsyncClient::config() const
  {
    return d->config;
  }

    QValueList<CDInfo>
  AsyncClient::lookupResponse() const
  {
    return d->lookupResponse;
  }

    void
  AsyncClient::lookup(const TrackOffsetList & trackOffsetList)
  {
    if (0 != d->helper)
    {
      kdDebug() << k_funcinfo << "Already running" << endl;
      emit(error(Unknown));
      return;
    }

    // Get the cddb id from trackOffsetList.

    d->cddbId = trackOffsetListToId(trackOffsetList);
    kdDebug() << k_funcinfo << "cddbId: " << d->cddbId << endl;

    if (!d->cddbId)
    {
      kdDebug() << "Can't create cddbid from offset list" << endl;
      emit(error(NoSuchCD));
      return;
    }

    // Try the cache if we're supposed to.

    if (
      CDDBLookupIgnoreCached != d->config.lookupTransport()
      &&
      HTTPLookupIgnoreCached != d->config.lookupTransport()
    )
    {
      kdDebug() << "Should try cache here" << endl;

      // FIXME - bad place to look.
      QFile f(QDir::homeDirPath() + "/.cddb/" + d->cddbId);

      if (f.exists())
      {
        // STUB
        kdDebug() << "Found locally cached info" << endl;
        emit(error(Unknown));
        return;
      }

      kdDebug() << "Didn't find locally cached info" << endl;
    }

    // If we're only supposed to try the cache and we failed, drop out now.

    if (CacheOnlyLookup == d->config.lookupTransport())
    {
      kdDebug() << "Only trying cache. Give up now." << endl;
      emit(error(NoSuchCD));
      return;
    }

    // Do the actual lookup.

    lookupWithHelper(trackOffsetList);
  }

    void
  AsyncClient::lookupWithHelper(const TrackOffsetList & trackOffsetList)
  {
    switch (d->config.lookupTransport())
    {
      case CDDBLookup:
      case CDDBLookupIgnoreCached:
        {
          AsyncCDDBLookup * helper = new AsyncCDDBLookup(this);

          d->helper = helper;

          connect(helper, SIGNAL(error(Error)), SLOT(slotError(Error)));
          connect
            (
              helper,
              SIGNAL(lookupResponseReady(const QValueList<CDInfo> &)),
              SLOT(slotLookupResponseReady(const QValueList<CDInfo> &))
            );

          helper->lookup
            (
              trackOffsetList,
              d->config.hostname(),
              d->config.port(),
              d->config.clientName(),
              d->config.clientVersion()
            );
        }
        break;

      case HTTPLookup:
      case HTTPLookupIgnoreCached:
#if 0

        AsyncHTTPLookup * helper = new AsyncHTTPLookup(this);

        d->helper = helper;

        connect(helper, SIGNAL(error(Error)), SLOT(slotError(Error)));
        connect
          (
            helper,
            SIGNAL(lookupResponseReady(const QValueList<CDInfo> &)),
            SLOT(slotLookupResponseReady(const QValueList<CDInfo> &))
          );

        helper->lookup
          (
            trackOffsetList,
            d->config.hostname(),
            d->config.port(),
            d->config.clientName(),
            d->config.clientVersion(),
            d->config.proxyEnabled(),
            d->config.proxyHostname(),
            d->config.proxyPort()
          );

#endif
        break;

      default:
        kdDebug() << k_funcinfo << "Unsupported transport: "
          << lookupTransportToString(d->config.lookupTransport()) << endl;
        emit(error(Unknown));
        break;
    }
  }

    void
  AsyncClient::submit(const CDInfo &)
  {
    kdDebug() << k_funcinfo << "STUB" << endl;
    emit(error(Unknown));
    return;
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

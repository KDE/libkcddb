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

#include "cache.h"
#include "asyncclient.h"
#include "asynccddblookup.h"
#include "asynchttplookup.h"

namespace KCDDB
{
  class AsyncClient::Private
  {
    public:

      QString               cddbId;
      Config                config;
      CDInfoList    cdInfoList;
      QObject             * helper;
  };

  AsyncClient::AsyncClient(QObject * parent, const char * name)
    : QObject(parent, name)
  {
    d = new Private;
    d->helper = 0;
    d->config.load();
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

    void
  AsyncClient::lookup(const TrackOffsetList & trackOffsetList)
  {
    if (0 != d->helper)
    {
      kdDebug() << k_funcinfo << "Already running" << endl;
      emit(result(Lookup::UnknownError));
      return;
    }

    // Get the cddb id from trackOffsetList.

    d->cddbId = trackOffsetListToId(trackOffsetList);
    kdDebug() << k_funcinfo << "cddbId: " << d->cddbId << endl;

    if (!d->cddbId)
    {
      kdDebug() << "Can't create cddbid from offset list" << endl;
      emit(result(Lookup::NoRecordFound));
      return;
    }

    if (Cache::Ignore != d->config.cachePolicy())
    {
      d->cdInfoList = Cache::lookup(trackOffsetList);

      if (!d->cdInfoList.isEmpty())
      {
        emit(result(Lookup::Success, d->cdInfoList));
        return;
      }
    }

    // If we're only supposed to try the cache and we failed, drop out now.

    if (Cache::Only == d->config.cachePolicy())
    {
      kdDebug() << "Only trying cache. Give up now." << endl;
      emit(result(Lookup::NoRecordFound));
      return;
    }

    // Do the actual lookup.

    lookupWithHelper(trackOffsetList);
  }

    void
  AsyncClient::lookupWithHelper(const TrackOffsetList & trackOffsetList)
  {
    AsyncLookup * helper = 0;

    switch (d->config.lookupTransport())
    {
      case Lookup::CDDB:
        helper = new AsyncCDDBLookup(this);
        break;

      case Lookup::HTTP:
        helper = new AsyncHTTPLookup(this);
        break;

      default:
        kdDebug() << k_funcinfo << "Unsupported transport: "
          << Lookup::transportToString(d->config.lookupTransport()) << endl;
        emit(result(Lookup::UnknownError));
        return;
    }

    d->helper = helper;

    connect
      (
        helper,
        SIGNAL(finished(Lookup::Result, const QValueList<CDinfo> &)),
        SLOT(slotLookupFinished(Lookup::Result, const QValueList<CDinfo> &))
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

    void
  AsyncClient::submit(const CDInfo &)
  {
    kdDebug() << k_funcinfo << "STUB" << endl;
    emit(result(Lookup::UnknownError));
    return;
  }

    void
  AsyncClient::slotLookupFinished
  (
    Lookup::Result resultCode,
    const CDInfoList & l
  )
  {
    emit(result(resultCode, l));
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

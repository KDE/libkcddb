/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002-2005 Benjamin C. Meyer <ben at meyerhome dot net>
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
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#include "client.h"

#include "synccddbplookup.h"
#include "asynccddbplookup.h"
#include "synchttplookup.h"
#include "asynchttplookup.h"
#include "syncsmtpsubmit.h"
#include "asyncsmtpsubmit.h"
#include "synchttpsubmit.h"
#include "asynchttpsubmit.h"
#include "cache.h"
#include "lookup.h"

#include "config-musicbrainz.h"
#ifdef HAVE_MUSICBRAINZ
#include "musicbrainz/musicbrainzlookup.h"
#include "musicbrainz/asyncmusicbrainzlookup.h"
#endif

#include <kdebug.h>

namespace KCDDB
{
  class Client::Private
  {
    public:

      Private()
        : cdInfoLookup(0),
          cdInfoSubmit(0),
          block( true )
      {}

      ~Private()
      {
        delete cdInfoLookup;
        delete cdInfoSubmit;
      }

      Lookup * cdInfoLookup;
      Submit * cdInfoSubmit;

      Config config;
      CDInfoList cdInfoList;
      TrackOffsetList trackOffsetList;
      bool block;
  };

  Client::Client()
    : d(new Private)
  {
    d->config.readConfig();
  }

  Client::~Client()
  {
    delete d;
  }

    Config &
  Client::config() const
  {
    return d->config;
  }

    void
  Client::setBlockingMode( bool enable )
  {
    d->block = enable;
  }

    bool
  Client::blockingMode() const
  {
    return d->block;
  }

    CDInfoList
  Client::lookupResponse() const
  {
    return d->cdInfoList;
  }

    Result
  Client::lookup(const TrackOffsetList & trackOffsetList)
  {
    d->cdInfoList.clear();
    d->trackOffsetList = trackOffsetList;

    if ( trackOffsetList.count() <= 1 )
    {
      kDebug(60010) << "Lookup called with empty offset list" << endl;
      return NoRecordFound;
    }

    if ( Cache::Ignore != d->config.cachePolicy() )
    {
      d->cdInfoList = Cache::lookup( trackOffsetList, config() );

      kDebug(60010) << "Found " << d->cdInfoList.count() << " hit(s)" << endl;

      if ( !d->cdInfoList.isEmpty() )
      {
        if ( !blockingMode() )
          emit finished( Success );

        return Success;
      }
    }

    if ( Cache::Only == d->config.cachePolicy() )
    {
      kDebug(60010) << "Only trying cache. Give up now." << endl;
      if ( !blockingMode() )
        emit finished( NoRecordFound );
      return NoRecordFound;
    }

    Result r;
    Lookup::Transport t = ( Lookup::Transport )d->config.lookupTransport();

    // just in case we have an info lookup hanging around, prevent mem leakage
    delete d->cdInfoLookup;

    if ( blockingMode() )
    {

      if( Lookup::CDDBP == t )
        d->cdInfoLookup = new SyncCDDBPLookup();
      else if ( Lookup::HTTP == t )
        d->cdInfoLookup = new SyncHTTPLookup();
      else
      {
#ifdef HAVE_MUSICBRAINZ
        d->cdInfoLookup = new MusicBrainzLookup();
#else
        kWarning() << "libkcddb not built with MusicBrainz support" << endl;
        return UnknownError;
#endif
      }

      r = d->cdInfoLookup->lookup( d->config.hostname(),
              d->config.port(), trackOffsetList );

      if ( Success == r )
      {
        d->cdInfoList = d->cdInfoLookup->lookupResponse();
        Cache::store( d->trackOffsetList, d->cdInfoList, config() );
      }

      delete d->cdInfoLookup;
      d->cdInfoLookup = 0L;
    }
    else
    {
      if( Lookup::CDDBP == t )
      {
        d->cdInfoLookup = new AsyncCDDBPLookup();

        connect( static_cast<AsyncCDDBPLookup *>( d->cdInfoLookup ),
                  SIGNAL( finished( KCDDB::Result ) ),
                  SLOT( slotFinished( KCDDB::Result ) ) );
      }
      else if ( Lookup::HTTP == t)
      {
        d->cdInfoLookup = new AsyncHTTPLookup();

        connect( static_cast<AsyncHTTPLookup *>( d->cdInfoLookup ),
                  SIGNAL( finished( KCDDB::Result ) ),
                  SLOT( slotFinished( KCDDB::Result ) ) );
      }
      else
      {
#ifdef HAVE_MUSICBRAINZ
        d->cdInfoLookup = new AsyncMusicBrainzLookup();

        connect( static_cast<AsyncMusicBrainzLookup *>( d->cdInfoLookup ),
                  SIGNAL( finished( KCDDB::Result ) ),
                  SLOT( slotFinished( KCDDB::Result ) ) );
#else
        kWarning() << "libkcddb not built with MusicBrainz support" << endl;
        return UnknownError;
#endif
      }

      r = d->cdInfoLookup->lookup( d->config.hostname(),
              d->config.port(), trackOffsetList );

      if ( Success != r )
      {
        delete d->cdInfoLookup;
        d->cdInfoLookup = 0L;
      }
    }

    return r;
  }

    void
  Client::slotFinished( Result r )
  {
    if ( d->cdInfoLookup && Success == r )
    {
      d->cdInfoList = d->cdInfoLookup->lookupResponse();
      Cache::store( d->trackOffsetList, d->cdInfoList, config() );
    }
    else
      d->cdInfoList.clear();

    emit finished( r );

    if ( d->cdInfoLookup ) // in case someone called lookup() while finished() was being processed, and deleted cdInfoLookup.
    {
      d->cdInfoLookup->deleteLater();
      d->cdInfoLookup = 0L;
    }
  }

    void
  Client::slotSubmitFinished( Result r )
  {
    emit finished( r );

    d->cdInfoSubmit->deleteLater();
    d->cdInfoSubmit=0L;
  }

    Result
  Client::submit(const CDInfo &cdInfo, const TrackOffsetList& offsetList)
  {
    // Check if it's valid

    if (!cdInfo.isValid())
      return CannotSave;

    uint last=0;
    for (int i=0; i < offsetList.count(); i++)
    {
      if(last >= offsetList[i])
        return CannotSave;
      last = offsetList[i];
    }

    //TODO Check that it is edited

    // just in case we have a cdInfoSubmit, prevent memory leakage
    delete d->cdInfoSubmit;

    QString from = d->config.emailAddress();

    switch (d->config.submitTransport())
    {
      case Submit::HTTP:
      {
        QString hostname = d->config.httpSubmitServer();
        uint port = d->config.httpSubmitPort();

        if ( blockingMode() )
          d->cdInfoSubmit = new SyncHTTPSubmit(from, hostname, port);
        else
        {
          d->cdInfoSubmit = new AsyncHTTPSubmit(from, hostname, port);
          connect( static_cast<AsyncHTTPSubmit *>( d->cdInfoSubmit ),
                  SIGNAL(finished( KCDDB::Result ) ),
                  SLOT( slotSubmitFinished( KCDDB::Result ) ) );
        }

        break;
      }
      case Submit::SMTP:
      {
        QString hostname = d->config.smtpHostname();
        uint port = d->config.smtpPort();
        QString username = d->config.smtpUsername();

        if ( blockingMode() )
          d->cdInfoSubmit = new SyncSMTPSubmit( hostname, port, username, from, d->config.submitAddress() );
        else
        {
          d->cdInfoSubmit = new AsyncSMTPSubmit( hostname, port, username, from, d->config.submitAddress() );
          connect( static_cast<AsyncSMTPSubmit *>( d->cdInfoSubmit ),
                  SIGNAL( finished( KCDDB::Result ) ),
                  SLOT( slotSubmitFinished( KCDDB::Result ) ) );
        }
        break;
      }
      default:
        kDebug(60010) << k_funcinfo << "Unsupported transport: " << endl;
//          << CDDB::transportToString(d->config.submitTransport()) << endl;
        return UnknownError;
        break;
    }

    Result r = d->cdInfoSubmit->submit( cdInfo, offsetList );

    if ( blockingMode() )
    {
      delete d->cdInfoSubmit;
      d->cdInfoSubmit = 0L;
    }

    return r;
  }

    void
  Client::store(const CDInfo &cdInfo, const TrackOffsetList& offsetList)
  {
    Cache::store(offsetList, cdInfo, config());
  }
}

#include "client.moc"


// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002-2005 Benjamin C. Meyer <ben at meyerhome dot net>
    SPDX-FileCopyrightText: 2003 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
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
#include "logging.h"

#include "config-musicbrainz.h"
#ifdef HAVE_MUSICBRAINZ5
#include "musicbrainz/musicbrainzlookup.h"
#include "musicbrainz/asyncmusicbrainzlookup.h"
#endif

namespace KCDDB
{
  class Client::Private
  {
    public:

      Private()
        : cdInfoLookup(nullptr),
          cdInfoSubmit(nullptr),
          block( true )
      {}

      ~Private()
      {
        delete cdInfoLookup;
        delete cdInfoSubmit;
        qDeleteAll(pendingLookups);
      }

      Lookup * cdInfoLookup;
      Submit * cdInfoSubmit;

      Config config;
      CDInfoList cdInfoList;
      TrackOffsetList trackOffsetList;
      QList<Lookup *> pendingLookups;
      bool block;
  };

  Client::Client()
    : d(new Private)
  {
    d->config.load();
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
	  qCDebug(LIBKCDDB) << "Lookup called with empty offset list";
      return NoRecordFound;
    }

    if ( d->config.cacheLookupEnabled() )
    {
      d->cdInfoList = Cache::lookup( trackOffsetList, config() );

	  qCDebug(LIBKCDDB) << "Found " << d->cdInfoList.count() << " hit(s)";

      if ( !d->cdInfoList.isEmpty() )
      {
        if ( !blockingMode() )
          Q_EMIT finished( Success );

        return Success;
      }
    }

    Result r = NoRecordFound;

    // just in case we have an info lookup hanging around, prevent mem leakage
    delete d->cdInfoLookup;
    d->cdInfoLookup = nullptr;
    qDeleteAll(d->pendingLookups);
    d->pendingLookups.clear();

    if ( blockingMode() )
    {
#ifdef HAVE_MUSICBRAINZ5
      if ( d->config.musicBrainzLookupEnabled() )
      {
        d->cdInfoLookup = new MusicBrainzLookup();

        r = d->cdInfoLookup->lookup( d->config.hostname(),
                d->config.port(), trackOffsetList );

        if ( Success == r )
        {
          d->cdInfoList = d->cdInfoLookup->lookupResponse();
          Cache::store( d->trackOffsetList, d->cdInfoList, config() );

          return r;
        }

        delete d->cdInfoLookup;
        d->cdInfoLookup = nullptr;
      }
#endif

      if ( d->config.freedbLookupEnabled() )
      {
        Lookup::Transport t = ( Lookup::Transport )d->config.freedbLookupTransport();
        if( Lookup::CDDBP == t )
          d->cdInfoLookup = new SyncCDDBPLookup();
        else
          d->cdInfoLookup = new SyncHTTPLookup();

        r = d->cdInfoLookup->lookup( d->config.hostname(),
                d->config.port(), trackOffsetList );

        if ( Success == r )
        {
          d->cdInfoList = d->cdInfoLookup->lookupResponse();
          Cache::store( d->trackOffsetList, d->cdInfoList, config() );

          return r;
        }

        delete d->cdInfoLookup;
        d->cdInfoLookup = nullptr;
      }

      return r;
    }
    else
    {
#ifdef HAVE_MUSICBRAINZ5
      if ( d->config.musicBrainzLookupEnabled() )
      {
        AsyncMusicBrainzLookup* lookup = new AsyncMusicBrainzLookup();

        connect( lookup, &AsyncMusicBrainzLookup::finished,
                 this, &Client::slotFinished );
        d->pendingLookups.append( lookup );
      }
#endif

      if ( d->config.freedbLookupEnabled() )
      {
        Lookup::Transport t = ( Lookup::Transport )d->config.freedbLookupTransport();

        if( Lookup::CDDBP == t )
        {
          AsyncCDDBPLookup* lookup = new AsyncCDDBPLookup();

          connect( lookup, &AsyncCDDBPLookup::finished,
                   this, &Client::slotFinished );
          d->pendingLookups.append( lookup );
        }
        else
        {
          AsyncHTTPLookup* lookup = new AsyncHTTPLookup();

          connect( lookup, &AsyncHTTPLookup::finished,
                   this, &Client::slotFinished );
          d->pendingLookups.append( lookup );
        }
      }

      return runPendingLookups();
    }
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

    if ( d->cdInfoLookup ) // in case someone called lookup() while finished() was being processed, and deleted cdInfoLookup.
    {
      d->cdInfoLookup->deleteLater();
      d->cdInfoLookup = nullptr;
    }

    if ( Success == r )
    {
      Q_EMIT finished( r );
      qDeleteAll( d->pendingLookups );
      d->pendingLookups.clear();
    }
    else
    {
      runPendingLookups();
    }
  }

    void
  Client::slotSubmitFinished( Result r )
  {
    Q_EMIT finished( r );

    d->cdInfoSubmit->deleteLater();
    d->cdInfoSubmit=nullptr;
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

    switch (d->config.freedbSubmitTransport())
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
                  &AsyncHTTPSubmit::finished,
                  this, &Client::slotSubmitFinished );
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
                  &AsyncSMTPSubmit::finished,
                  this, &Client::slotSubmitFinished );
        }
        break;
      }
      default:
		qCDebug(LIBKCDDB) << "Unsupported transport: ";
//          << CDDB::transportToString(d->config.submitTransport()) << endl;
        return UnknownError;
        break;
    }

    Result r = d->cdInfoSubmit->submit( cdInfo, offsetList );

    if ( blockingMode() )
    {
      delete d->cdInfoSubmit;
      d->cdInfoSubmit = nullptr;
    }

    return r;
  }

    Result
  Client::runPendingLookups()
  {
    if (!d->pendingLookups.empty())
    {
      d->cdInfoLookup = d->pendingLookups.takeFirst();

      Result r = d->cdInfoLookup->lookup( d->config.hostname(),
              d->config.port(), d->trackOffsetList );

      if ( Success != r )
      {
        delete d->cdInfoLookup;
        d->cdInfoLookup = nullptr;
      }

      return r;
    }
    else
    {
      Q_EMIT finished( NoRecordFound );
      return NoRecordFound;
    }
  }

    void
  Client::store(const CDInfo &cdInfo, const TrackOffsetList& offsetList)
  {
    Cache::store(offsetList, cdInfo, config());
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

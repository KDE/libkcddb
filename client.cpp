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
#endif

#include <kdebug.h>

namespace KCDDB
{
  class Client::Private
  {
    public:

      Private()
        : block( true )
      {}

      Config config;
      CDInfoList cdInfoList;
      bool block;
  };

  Client::Client()
    : QObject(),
      cdInfoLookup(0),
      cdInfoSubmit(0)
  {
    d = new Private;
    d->config.readConfig();
  }

  Client::~Client()
  {
    delete d;
    delete cdInfoLookup;
    delete cdInfoSubmit;
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

    CDDB::Result
  Client::lookup(const TrackOffsetList & trackOffsetList)
  {
    d->cdInfoList.clear();

    if ( trackOffsetList.count() <= 1 )
    {
      kDebug(60010) << "Lookup called with empty offset list" << endl;
      return Lookup::NoRecordFound;
    }

    QString cddbId = Lookup::trackOffsetListToId( trackOffsetList );

    if ( Cache::Ignore != d->config.cachePolicy() )
    {
      d->cdInfoList = Cache::lookup( cddbId, config() );

      kDebug(60010) << "Found " << d->cdInfoList.count() << " hit(s)" << endl;

      if ( !d->cdInfoList.isEmpty() )
      {
        if ( !blockingMode() )
          emit finished( Lookup::Success );

        return CDDB::Success;
      }
    }

    if ( Cache::Only == d->config.cachePolicy() )
    {
      kDebug(60010) << "Only trying cache. Give up now." << endl;
      if ( !blockingMode() )
        emit finished( Lookup::NoRecordFound );
      return CDDB::NoRecordFound;
    }

    CDDB::Result r;
    Lookup::Transport t = ( Lookup::Transport )d->config.lookupTransport();

    // just in case we have an info lookup hanging around, prevent mem leakage
    delete cdInfoLookup;

    if ( blockingMode() )
    {

      if( Lookup::CDDBP == t )
        cdInfoLookup = new SyncCDDBPLookup();
      else if ( Lookup::HTTP == t )
        cdInfoLookup = new SyncHTTPLookup();
      else
      {
#ifdef HAVE_MUSICBRAINZ
        cdInfoLookup = new MusicBrainzLookup();
#else
        kWarning() << "libkcddb not built with MusicBrainz support" << endl;
        return CDDB::UnknownError;
#endif
      }

      r = cdInfoLookup->lookup( d->config.hostname(),
              d->config.port(), trackOffsetList );

      if ( CDDB::Success == r )
      {
        d->cdInfoList = cdInfoLookup->lookupResponse();
        Cache::store( d->cdInfoList, config() );
      }

      delete cdInfoLookup;
      cdInfoLookup = 0L;
    }
    else
    {
      if( Lookup::CDDBP == t )
      {
        cdInfoLookup = new AsyncCDDBPLookup();

        connect( static_cast<AsyncCDDBPLookup *>( cdInfoLookup ),
                  SIGNAL( finished( CDDB::Result ) ),
                  SLOT( slotFinished( CDDB::Result ) ) );
      }
      else if ( Lookup::HTTP == t)
      {
        cdInfoLookup = new AsyncHTTPLookup();

        connect( static_cast<AsyncHTTPLookup *>( cdInfoLookup ),
                  SIGNAL( finished( CDDB::Result ) ),
                  SLOT( slotFinished( CDDB::Result ) ) );
      }
      else
      {
#ifdef HAVE_MUSICBRAINZ
        cdInfoLookup = new MusicBrainzLookup();

        connect( static_cast<MusicBrainzLookup *>( cdInfoLookup ),
                  SIGNAL( finished( CDDB::Result ) ),
                  SLOT( slotFinished( CDDB::Result ) ) );
#else
        kWarning() << "libkcddb not built with MusicBrainz support" << endl;
        return CDDB::UnknownError;
#endif
      }

      r = cdInfoLookup->lookup( d->config.hostname(),
              d->config.port(), trackOffsetList );

      if ( Lookup::Success != r )
      {
        delete cdInfoLookup;
        cdInfoLookup = 0L;
      }
    }

    return r;
  }

    void
  Client::slotFinished( CDDB::Result r )
  {
    if ( cdInfoLookup && CDDB::Success == r )
    {
      d->cdInfoList = cdInfoLookup->lookupResponse();
      Cache::store( d->cdInfoList, config() );
    }
    else
      d->cdInfoList.clear();

    emit finished( r );

    if ( cdInfoLookup ) // in case someone called lookup() while finished() was being processed, and deleted cdInfoLookup.
    {
      cdInfoLookup->deleteLater();
      cdInfoLookup = 0L;
    }
  }

    void
  Client::slotSubmitFinished( CDDB::Result r )
  {
    emit finished( r );

    cdInfoSubmit->deleteLater();
    cdInfoSubmit=0L;
  }

    CDDB::Result
  Client::submit(const CDInfo &cdInfo, const TrackOffsetList& offsetList)
  {
    // Check if it's valid

    if (!cdInfo.isValid())
      return CDDB::CannotSave;

    uint last=0;
    for (int i=0; i < offsetList.count(); i++)
    {
      if(last >= offsetList[i])
        return CDDB::CannotSave;
      last = offsetList[i];
    }

    //TODO Check that it is edited

    // just in case we have a cdInfoSubmit, prevent memory leakage
    delete cdInfoSubmit;

    QString from = d->config.emailAddress();

    switch (d->config.submitTransport())
    {
      case Submit::HTTP:
      {
        QString hostname = d->config.httpSubmitServer();
        uint port = d->config.httpSubmitPort();

        if ( blockingMode() )
          cdInfoSubmit = new SyncHTTPSubmit(from, hostname, port);
        else
        {
          cdInfoSubmit = new AsyncHTTPSubmit(from, hostname, port);
          connect( static_cast<AsyncHTTPSubmit *>( cdInfoSubmit ),
                  SIGNAL(finished( CDDB::Result ) ),
                  SLOT( slotSubmitFinished( CDDB::Result ) ) );
        }
        
        break;
      }
      case Submit::SMTP:
      {
        QString hostname = d->config.smtpHostname();
        uint port = d->config.smtpPort();
        QString username = d->config.smtpUsername();

        if ( blockingMode() )
          cdInfoSubmit = new SyncSMTPSubmit( hostname, port, username, from, d->config.submitAddress() );
        else
        {
          cdInfoSubmit = new AsyncSMTPSubmit( hostname, port, username, from, d->config.submitAddress() );
          connect( static_cast<AsyncSMTPSubmit *>( cdInfoSubmit ),
                  SIGNAL( finished( CDDB::Result ) ),
                  SLOT( slotSubmitFinished( CDDB::Result ) ) );
        }
        break;
      }
      default:
        kDebug(60010) << k_funcinfo << "Unsupported transport: " << endl;
//          << CDDB::transportToString(d->config.submitTransport()) << endl;
        return CDDB::UnknownError;
        break;
    }
    
    CDDB::Result r = cdInfoSubmit->submit( cdInfo, offsetList );

    if ( blockingMode() )
    {
      delete cdInfoSubmit;
      cdInfoSubmit = 0L;
    }

    return r;
  }
    
    void
  Client::store(const CDInfo &cdInfo)
  {
    Cache::store(cdInfo, config());
  }
}

#include "client.moc"


// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

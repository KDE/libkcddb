/*
    SPDX-FileCopyrightText: 2006 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "asyncmusicbrainzlookup.h"

#include "musicbrainzlookup.h"

#include <QDebug>
#include <QThread>

namespace KCDDB
{
  class LookupThread : public QThread
  {

  Q_OBJECT

  public:
    void run() override
    {
      Result result;
      CDInfoList lookupResponse;
      MusicBrainzLookup lookup;

      result = lookup.lookup(QString(), 0, m_offsetList);

      if (result == Success)
        lookupResponse = lookup.lookupResponse();

      Q_EMIT lookupFinished(result, lookupResponse);
    }

    TrackOffsetList m_offsetList;

  Q_SIGNALS:
    void lookupFinished( KCDDB::Result, KCDDB::CDInfoList );
  };

  AsyncMusicBrainzLookup::AsyncMusicBrainzLookup()
  {
    // Register custom data types for the signal-slot connection with the lookup thread:
    qRegisterMetaType<KCDDB::Result>("KCDDB::Result");
    qRegisterMetaType<KCDDB::CDInfoList>("KCDDB::CDInfoList");
  }

  AsyncMusicBrainzLookup::~AsyncMusicBrainzLookup()
  {

  }

  Result AsyncMusicBrainzLookup::lookup( const QString &, uint, const TrackOffsetList & trackOffsetList )
  {
    LookupThread* lookupThread = new LookupThread();
    lookupThread->m_offsetList = trackOffsetList;
    connect(lookupThread, &LookupThread::lookupFinished, this, &AsyncMusicBrainzLookup::processLookupResult); // queued connection

    // Make the thread object "self-destructive"; allows us to keep the destructor non-blocking
    connect(lookupThread, &LookupThread::finished, lookupThread, &LookupThread::deleteLater);
    // NOTE: the memory automatically gets cleared after the thread has finished

    lookupThread->start();

    return Success;
  }

  void AsyncMusicBrainzLookup::processLookupResult( KCDDB::Result result, KCDDB::CDInfoList lookupResponse )
  {
    qDebug();

    cdInfoList_ = lookupResponse;

    Q_EMIT finished(result);
  }
}

#include "asyncmusicbrainzlookup.moc"
#include "moc_asyncmusicbrainzlookup.cpp"

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

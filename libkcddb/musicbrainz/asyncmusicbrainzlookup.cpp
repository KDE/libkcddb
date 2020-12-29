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
  public:
    void run() override
    {
      MusicBrainzLookup lookup;

      m_result = lookup.lookup(QString(), 0, m_offsetList);

      if (m_result == Success)
        m_lookupResponse = lookup.lookupResponse();
    }

    TrackOffsetList m_offsetList;
    Result m_result;
    CDInfoList m_lookupResponse;
  } ;

  AsyncMusicBrainzLookup::AsyncMusicBrainzLookup()
  {

  }

  AsyncMusicBrainzLookup::~AsyncMusicBrainzLookup()
  {
    delete m_lookupThread;
  }

  Result AsyncMusicBrainzLookup::lookup( const QString &, uint, const TrackOffsetList & trackOffsetList )
  {
    m_lookupThread = new LookupThread();
    m_lookupThread->m_offsetList = trackOffsetList;
    connect(m_lookupThread, &QThread::finished, this, &AsyncMusicBrainzLookup::lookupFinished);

    m_lookupThread->start();

    return Success;
  }

  void AsyncMusicBrainzLookup::lookupFinished()
  {
    qDebug() ;

    cdInfoList_ = m_lookupThread->m_lookupResponse;

    emit finished(m_lookupThread->m_result);
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

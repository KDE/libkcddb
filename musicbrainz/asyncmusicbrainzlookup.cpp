/*
  Copyright (C) 2006 Richard Lärkäng <nouseforaname@home.se>

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

#include "asyncmusicbrainzlookup.h"
#include "musicbrainzlookup.h"
#include <QtCore/QThread>

#include <kdebug.h>

namespace KCDDB
{
  class LookupThread : public QThread
  {
  public:
    void run()
    {
      MusicBrainzLookup lookup;

      m_result = lookup.lookup("", 0, m_offsetList);

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
    connect(m_lookupThread, SIGNAL(finished()), SLOT(lookupFinished()));

    m_lookupThread->start();

    return Success;
  }

  void AsyncMusicBrainzLookup::lookupFinished()
  {
    kDebug() << k_funcinfo << endl;

    cdInfoList_ = m_lookupThread->m_lookupResponse;

    emit finished(m_lookupThread->m_result);
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

#include "asyncmusicbrainzlookup.moc"

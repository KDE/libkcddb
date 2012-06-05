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


#include <qtest_kde.h>
#include "asyncmusicbrainztest.h"
#include "libkcddb/cache.h"
#include "libkcddb/lookup.h"
#include "config-musicbrainz.h"

void AsyncMusicBrainzTest::testLookup()
{
  using namespace KCDDB;

#ifndef HAVE_MUSICBRAINZ5
  QSKIP("This test requires libmusicbrainz", SkipAll);
#endif

  client_ = new Client;
  client_->config().setCacheLookupEnabled(false);
  client_->config().setFreedbLookupEnabled(false);
  client_->config().setMusicBrainzLookupEnabled(true);
  client_->setBlockingMode( false );

  connect(client_,SIGNAL(finished(KCDDB::Result)),SLOT(slotFinished(KCDDB::Result)));

  TrackOffsetList list;

  // Various Artists / Definitivt 50 Spänn 10
  list
    << 150
    << 17130
    << 26340
    << 44855
    << 60530
    << 75800
    << 90182
    << 106605
    << 122995
    << 139005
    << 153317
    << 167967
    << 179012
    << 190190
    << 202162
    << 211312
    << 229037
    << 240035;

  client_->lookup(list);

  m_eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

  QVERIFY(m_result == Success);

  // See http://musicbrainz.org/release/dbb3e39a-4bea-4e32-a546-456654f30ca6.html for changes
  QCOMPARE(m_info.numberOfTracks(),17);

  QCOMPARE(m_info.get(Artist).toString(),QString("Various Artists"));
  QCOMPARE(m_info.get(Title).toString(),QString::fromUtf8("Definitivt 50 spänn 10"));
  // genre not really supported for musicbrainz
  QCOMPARE(m_info.get(Genre).toString(),QString());
  QCOMPARE(m_info.get(Year).toInt(),2001);
  QCOMPARE(m_info.track(0).get(Title).toString(),QString::fromUtf8("Uphill Struggle"));
  QCOMPARE(m_info.track(1).get(Title).toString(),QString::fromUtf8("Vill du bli miljonär?"));
  QCOMPARE(m_info.track(2).get(Title).toString(),QString::fromUtf8("Här är jag"));
  QCOMPARE(m_info.track(3).get(Title).toString(),QString::fromUtf8("Århundradets fest"));
  QCOMPARE(m_info.track(4).get(Title).toString(),QString::fromUtf8("Vem valde mittemellan"));
  QCOMPARE(m_info.track(5).get(Title).toString(),QString::fromUtf8("Hippieambulansen"));
  QCOMPARE(m_info.track(6).get(Title).toString(),QString::fromUtf8("Jonsson"));
  QCOMPARE(m_info.track(7).get(Title).toString(),QString::fromUtf8("Jag lämnar filmen"));
  QCOMPARE(m_info.track(8).get(Title).toString(),QString::fromUtf8("39 Steg"));
  QCOMPARE(m_info.track(9).get(Title).toString(),QString::fromUtf8("Lyckliga land"));
  QCOMPARE(m_info.track(10).get(Title).toString(),QString::fromUtf8("Helpless"));
  QCOMPARE(m_info.track(11).get(Title).toString(),QString::fromUtf8("Tunna skivor"));
  QCOMPARE(m_info.track(12).get(Title).toString(),QString::fromUtf8("God läskeblask"));
  QCOMPARE(m_info.track(13).get(Title).toString(),QString::fromUtf8("Hur snygg é du då?"));
  QCOMPARE(m_info.track(14).get(Title).toString(),QString::fromUtf8("Självupptagen"));
  QCOMPARE(m_info.track(15).get(Title).toString(),QString::fromUtf8("Sound Is Compressed; Words Rebel and Hiss"));
  QCOMPARE(m_info.track(16).get(Title).toString(),QString::fromUtf8("Vi är på väg"));
  QCOMPARE(m_info.track(0).get(Artist).toString(),QString::fromUtf8("Adhesive"));
  QCOMPARE(m_info.track(1).get(Artist).toString(),QString::fromUtf8("Troublemakers"));
  QCOMPARE(m_info.track(2).get(Artist).toString(),QString::fromUtf8("Mimikry"));
  QCOMPARE(m_info.track(3).get(Artist).toString(),QString::fromUtf8("Lundberg & Dellamorte"));
  QCOMPARE(m_info.track(4).get(Artist).toString(),QString::fromUtf8("Charta 77"));
  QCOMPARE(m_info.track(5).get(Artist).toString(),QString::fromUtf8("Ubba"));
  QCOMPARE(m_info.track(6).get(Artist).toString(),QString::fromUtf8("Coca Carola"));
  QCOMPARE(m_info.track(7).get(Artist).toString(),QString::fromUtf8("Spoiler"));
  QCOMPARE(m_info.track(8).get(Artist).toString(),QString::fromUtf8("Strebers"));
  QCOMPARE(m_info.track(9).get(Artist).toString(),QString::fromUtf8("Iguana Party"));
  QCOMPARE(m_info.track(10).get(Artist).toString(),QString::fromUtf8("Stoned"));
  QCOMPARE(m_info.track(11).get(Artist).toString(),QString::fromUtf8("Per Bertil Birgers orkester"));
  QCOMPARE(m_info.track(12).get(Artist).toString(),QString::fromUtf8("Slutstation Tjernobyl"));
  QCOMPARE(m_info.track(13).get(Artist).toString(),QString::fromUtf8("SBD"));
  QCOMPARE(m_info.track(14).get(Artist).toString(),QString::fromUtf8("Skumdum"));
  QCOMPARE(m_info.track(15).get(Artist).toString(),QString::fromUtf8("Text"));
  QCOMPARE(m_info.track(16).get(Artist).toString(),QString::fromUtf8("Ohlson Har Semester Production"));
  // comments not supported in a simple way
  for (int j=0; j < 17; j++)
    QCOMPARE(m_info.track(j).get(Comment).toString(),QString());

  // Make sure it's the same when loaded from the cache again
  client_->config().setCacheLookupEnabled(true);
  client_->config().setFreedbLookupEnabled(false);
  client_->config().setMusicBrainzLookupEnabled(false);
  client_->setBlockingMode(true);

  client_->lookup(list);

  CDInfoList response = client_->lookupResponse();
  QVERIFY(response.count() > 0);

  CDInfo cacheInfo(response.first());
  QCOMPARE(m_info.get(Artist).toString(),cacheInfo.get(Artist).toString());
  QCOMPARE(m_info.get(Title).toString(),cacheInfo.get(Title).toString());
  QCOMPARE(m_info.get(Year).toInt(),cacheInfo.get(Year).toInt());
  for (int j=0; j < 17; j++)
  {
    QCOMPARE(m_info.track(j).get(Title).toString(),cacheInfo.track(j).get(Title).toString());
    QCOMPARE(m_info.track(j).get(Comment).toString(),cacheInfo.track(j).get(Comment).toString());
  }
}

  void
AsyncMusicBrainzTest::slotFinished(Result r)
{
  m_result = r;
  kDebug() << ": Got " << KCDDB::resultToString(r);

  CDInfoList l = client_->lookupResponse();

  kDebug() << ": Item count: " <<  l.count();

  QVERIFY(l.count() > 0);
  m_info = l.first();

  m_eventLoop.quit();
}

QTEST_KDEMAIN(AsyncMusicBrainzTest, NoGUI)

#include "asyncmusicbrainztest.moc"

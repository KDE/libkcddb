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
#include "asynccddblookuptest.h"
#include "libkcddb/cache.h"
#include "libkcddb/lookup.h"

void AsyncCDDBLookupTest::testLookup()
{
  using namespace KCDDB;

  client_ = new Client;
  client_->config().setHostname("freedb.freedb.org");
  client_->config().setPort(8880);
  client_->config().setCacheLookupEnabled(false);
  client_->config().setFreedbLookupEnabled(true);
  client_->config().setMusicBrainzLookupEnabled(false);
  client_->config().setFreedbLookupTransport(Lookup::CDDBP);
  client_->setBlockingMode( false );

  connect(client_,SIGNAL(finished(KCDDB::Result)),SLOT(slotFinished(KCDDB::Result)));

  TrackOffsetList list;

  // a1107d0a - Kruder & Dorfmeister - The K&D Sessions - Disc One.
  list
    << 150      // First track start.
    << 29462
    << 66983
    << 96785
    << 135628
    << 168676
    << 194147
    << 222158
    << 247076
    << 278203   // Last track start.
    << 316732;  // Disc end.

  client_->lookup(list);

  m_eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

  delete client_;

  QVERIFY(m_result == Success);

  // If revision doesn't match, test probably needs to be updated
  // See: http://www.freedb.org/freedb/jazz/a1107d0a for updated data
  QCOMPARE(m_info.get("revision").toInt(), 5);

  QCOMPARE(m_info.get(Artist).toString(),QString("Kruder & Dorfmeister"));
  QCOMPARE(m_info.get(Title).toString(),QString("The K&D Sessions - CD1"));
  QCOMPARE(m_info.get(Genre).toString(),QString("Tribal"));
  QCOMPARE(m_info.get(Year).toInt(),1998);
  QCOMPARE(m_info.track(0).get(Title).toString(),QString("Roni Size - Heroes (Kruder's Long Loose Bossa)"));
  QCOMPARE(m_info.track(1).get(Title).toString(),QString("Alex Reece - Jazz Master (K&D Session)"));
  QCOMPARE(m_info.track(2).get(Title).toString(),QString("Count Basic - Speechless (Drum 'N' Bass)"));
  QCOMPARE(m_info.track(3).get(Title).toString(),QString("Rocker's HiFi - Going Under (Main Version - K&D Session)"));
  QCOMPARE(m_info.track(4).get(Title).toString(),QString("Bomb The Bass - Bug Powder Dust (K&D Session)"));
  QCOMPARE(m_info.track(5).get(Title).toString(),QString("Aphrodelics - Rollin' On Chrome (Wild Motherfucker Dub)"));
  QCOMPARE(m_info.track(6).get(Title).toString(),QString("Depeche Mode - Useless (K&D Session)"));
  QCOMPARE(m_info.track(7).get(Title).toString(),QString("Count Basic - Gotta Jazz (Dorfmeister Remix)"));
  QCOMPARE(m_info.track(8).get(Title).toString(),QString::fromUtf8("Trüby Trio - Donaueschingen (Peter Kruder's Donaudampfschifffahrtsgesellschaftskapitaenskajuetenremix)"));
  QCOMPARE(m_info.track(9).get(Title).toString(),QString("Lamb - Trans Fatty Acid (K&D Session)"));
  QCOMPARE(m_info.track(0).get(Artist).toString(),QString("Kruder & Dorfmeister"));
  QCOMPARE(m_info.track(1).get(Artist).toString(),QString("Kruder & Dorfmeister"));
  QCOMPARE(m_info.track(2).get(Artist).toString(),QString("Kruder & Dorfmeister"));
  QCOMPARE(m_info.track(3).get(Artist).toString(),QString("Kruder & Dorfmeister"));
  QCOMPARE(m_info.track(4).get(Artist).toString(),QString("Kruder & Dorfmeister"));
  QCOMPARE(m_info.track(5).get(Artist).toString(),QString("Kruder & Dorfmeister"));
  QCOMPARE(m_info.track(6).get(Artist).toString(),QString("Kruder & Dorfmeister"));
  QCOMPARE(m_info.track(7).get(Artist).toString(),QString("Kruder & Dorfmeister"));
  QCOMPARE(m_info.track(8).get(Artist).toString(),QString("Kruder & Dorfmeister"));
  QCOMPARE(m_info.track(9).get(Artist).toString(),QString("Kruder & Dorfmeister"));
}

  void
AsyncCDDBLookupTest::slotFinished(Result r)
{
  m_result = r;
  kDebug() << "AsyncCDDBLookupTest::slotResult: Got " << KCDDB::resultToString(r);

  CDInfoList l = client_->lookupResponse();

  kDebug() << "AsyncCDDBLookupTest::slotResult: Item count: " <<  l.count();

  foreach(const CDInfo &i, l)
  {
    if (i.get("discid") == "a1107d0a" && i.get(Category) == "jazz")
    {
      kDebug() << "Found the CD";
      m_info = i;
      break;
    }
  }

  m_eventLoop.quit();
}

QTEST_KDEMAIN(AsyncCDDBLookupTest, NoGUI)

#include "asynccddblookuptest.moc"

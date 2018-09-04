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

#include "musicbrainztest.h"
#include "libkcddb/client.h"
#include "libkcddb/cache.h"
#include "libkcddb/lookup.h"
#include "config-musicbrainz.h"
#include <QTest>

void MusicBrainzTest::testLookup()
{
  using namespace KCDDB;

#ifndef HAVE_MUSICBRAINZ5
  QSKIP("This test requires libmusicbrainz", SkipAll);
#endif

  Client c;
  c.config().setCacheLookupEnabled(false);
  c.config().setFreedbLookupEnabled(false);
  c.config().setMusicBrainzLookupEnabled(true);

  TrackOffsetList list;

  //The Liptones / The Latest News
  list
    << 150
    << 9219
    << 20386
    << 34134
    << 51437
    << 68809
    << 86591
    << 106288
    << 115568
    << 133312
    << 155593
    << 169832
    << 179626
    << 194958
    << 212506;

  Result r = c.lookup(list);

  QVERIFY(r == Success);

  qDebug() << "Client::lookup gave : " << resultToString(r);

  CDInfoList response = c.lookupResponse();

  qDebug() << "Client::lookup returned : " << response.count() << " entries"
    << endl;

  QVERIFY(response.count() > 0);

  // See http://musicbrainz.org/release/68ec4a09-576c-472e-9103-aed99b6a3ffc for changes
  CDInfo i(response.first());
  QCOMPARE(i.numberOfTracks(),14);

  QCOMPARE(i.get(Artist).toString(),QString::fromUtf8("The Liptones"));
  QCOMPARE(i.get(Title).toString(),QString::fromUtf8("The Latest News"));
  // genre not really supported for musicbrainz
  QCOMPARE(i.get(Genre).toString(),QString());
  QCOMPARE(i.get(Year).toInt(),2002);
  QCOMPARE(i.track(0).get(Title).toString(),QString::fromUtf8("Jungle Heat"));
  QCOMPARE(i.track(1).get(Title).toString(),QString::fromUtf8("It's All I Hear You Say"));
  QCOMPARE(i.track(2).get(Title).toString(),QString::fromUtf8("Girl for Tonight"));
  QCOMPARE(i.track(3).get(Title).toString(),QString::fromUtf8("Shoot Em Down"));
  QCOMPARE(i.track(4).get(Title).toString(),QString::fromUtf8("Beautiful Day"));
  QCOMPARE(i.track(5).get(Title).toString(),QString::fromUtf8("Paranoia"));
  QCOMPARE(i.track(6).get(Title).toString(),QString::fromUtf8("My Way"));
  QCOMPARE(i.track(7).get(Title).toString(),QString::fromUtf8("Dressed in Pink"));
  QCOMPARE(i.track(8).get(Title).toString(),QString::fromUtf8("No Way Out"));
  QCOMPARE(i.track(9).get(Title).toString(),QString::fromUtf8("Strange Kind of Justice"));
  QCOMPARE(i.track(10).get(Title).toString(),QString::fromUtf8("Waiting Til Midnight"));
  QCOMPARE(i.track(11).get(Title).toString(),QString::fromUtf8("Liptones"));
  QCOMPARE(i.track(12).get(Title).toString(),QString::fromUtf8("Out With the Boys"));
  QCOMPARE(i.track(13).get(Title).toString(),QString::fromUtf8("Free Like a Bird"));
  // comments not supported in a simple way
  for (int j=0; j < 14; j++)
    QCOMPARE(i.track(j).get(Comment).toString(),QString());

  // Make sure it's the same when loaded from the cache again
  c.config().setCacheLookupEnabled(true);
  c.config().setFreedbLookupEnabled(false);
  c.config().setMusicBrainzLookupEnabled(false);

  c.lookup(list);

  response = c.lookupResponse();
  QVERIFY(response.count() > 0);

  CDInfo cacheInfo(response.first());
  QCOMPARE(i.get(Artist).toString(),cacheInfo.get(Artist).toString());
  QCOMPARE(i.get(Title).toString(),cacheInfo.get(Title).toString());
  QCOMPARE(i.get(Year).toInt(),cacheInfo.get(Year).toInt());
  for (int j=0; j < 14; j++)
  {
    QCOMPARE(i.track(j).get(Title).toString(),cacheInfo.track(j).get(Title).toString());
    QCOMPARE(i.track(j).get(Comment).toString(),cacheInfo.track(j).get(Comment).toString());
  }
}

QTEST_GUILESS_MAIN(MusicBrainzTest)

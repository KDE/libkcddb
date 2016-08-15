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

#include "musicbrainztest-fulldate.h"
#include "libkcddb/client.h"
#include "libkcddb/cache.h"
#include "libkcddb/lookup.h"
#include "config-musicbrainz.h"
#include <QtTest/QTest>

void MusicBrainzTestFullDate::testLookup()
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

  list <<
    150 <<
    11837 <<
    20687 <<
    29120 <<
    35557 <<
    50087 <<
    58550 <<
    72575 <<
    81887 <<
    95745 <<
    104340 <<
    115425 <<
    124285 <<
    132317 <<
    142692 <<
    160272 <<
    169325 <<
    181377 <<
    194482 <<
    205747 <<
    216492 <<
    229507 <<
    242715 <<
    252112 <<
    274965 <<
    285820 <<
    308957 <<
    322362 <<
    338662 <<
    350172 <<
    356692;

  Result r = c.lookup(list);

  QVERIFY(r == Success);

  qDebug() << "Client::lookup gave : " << resultToString(r);

  CDInfoList response = c.lookupResponse();

  qDebug() << "Client::lookup returned : " << response.count() << " entries"
    << endl;

  QVERIFY(response.count() > 0);

  // See http://musicbrainz.org/release/04319e80-0948-4e89-8180-7cf6f17fc77a for changes
  CDInfo i(response.first());
  QCOMPARE(i.numberOfTracks(),30);

  QCOMPARE(i.get(Artist).toString(),QString::fromUtf8("Troublemakers"));
  QCOMPARE(i.get(Title).toString(),QString::fromUtf8("30"));
  // genre not really supported for musicbrainz
  QCOMPARE(i.get(Genre).toString(),QString());
  QCOMPARE(i.get(Year).toInt(),2011);
  QCOMPARE(i.track(0).get(Title).toString(),QString::fromUtf8("30"));
  QCOMPARE(i.track(1).get(Title).toString(),QString::fromUtf8("Staden Göteborg"));
  QCOMPARE(i.track(2).get(Title).toString(),QString::fromUtf8("Lilla Blå"));
  QCOMPARE(i.track(3).get(Title).toString(),QString::fromUtf8("Kleptoman"));
  QCOMPARE(i.track(4).get(Title).toString(),QString::fromUtf8("Grogg å parabol"));
  QCOMPARE(i.track(5).get(Title).toString(),QString::fromUtf8("Majorna brinner"));
  QCOMPARE(i.track(6).get(Title).toString(),QString::fromUtf8("Utan hjärta stannar Sverige"));
  QCOMPARE(i.track(7).get(Title).toString(),QString::fromUtf8("Dom e efter mig"));
  QCOMPARE(i.track(8).get(Title).toString(),QString::fromUtf8("Mammas flickor"));
  QCOMPARE(i.track(9).get(Title).toString(),QString::fromUtf8("Handgranat"));
  QCOMPARE(i.track(10).get(Title).toString(),QString::fromUtf8("Stenar o blod"));
  QCOMPARE(i.track(11).get(Title).toString(),QString::fromUtf8("Enkel Berlin"));
  QCOMPARE(i.track(12).get(Title).toString(),QString::fromUtf8("Dom ljuger"));
  QCOMPARE(i.track(13).get(Title).toString(),QString::fromUtf8("Jobba"));
  QCOMPARE(i.track(14).get(Title).toString(),QString::fromUtf8("Varför lyser inga stjärnor"));
  QCOMPARE(i.track(15).get(Title).toString(),QString::fromUtf8("Partytrash"));
  QCOMPARE(i.track(16).get(Title).toString(),QString::fromUtf8("Förstod ingenting"));
  QCOMPARE(i.track(17).get(Title).toString(),QString::fromUtf8("Bara om min älskade väntar"));
  QCOMPARE(i.track(18).get(Title).toString(),QString::fromUtf8("Lillebror"));
  QCOMPARE(i.track(19).get(Title).toString(),QString::fromUtf8("Skandaler"));
  QCOMPARE(i.track(20).get(Title).toString(),QString::fromUtf8("Hjältarna skålar i blod"));
  QCOMPARE(i.track(21).get(Title).toString(),QString::fromUtf8("Bomber o granater"));
  QCOMPARE(i.track(22).get(Title).toString(),QString::fromUtf8("Vill du bli miljonär"));
  QCOMPARE(i.track(23).get(Title).toString(),QString::fromUtf8("Stjärnorna kvittar det lika"));
  QCOMPARE(i.track(24).get(Title).toString(),QString::fromUtf8("Naken"));
  QCOMPARE(i.track(25).get(Title).toString(),QString::fromUtf8("Månen lyser klar"));
  QCOMPARE(i.track(26).get(Title).toString(),QString::fromUtf8("Vild o vacker"));
  QCOMPARE(i.track(27).get(Title).toString(),QString::fromUtf8("Jag fryser"));
  QCOMPARE(i.track(28).get(Title).toString(),QString::fromUtf8("Ein zwei polizei"));
  QCOMPARE(i.track(29).get(Title).toString(),QString::fromUtf8("Jävla idiot"));
  for (int j=0; j < 30; j++)
    QCOMPARE(i.track(j).get(Artist).toString(),QString::fromUtf8("Troublemakers"));
  // comments not supported in a simple way
  for (int j=0; j < 19; j++)
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
  for (int j=0; j < 19; j++)
  {
    QCOMPARE(i.track(j).get(Title).toString(),cacheInfo.track(j).get(Title).toString());
    QCOMPARE(i.track(j).get(Comment).toString(),cacheInfo.track(j).get(Comment).toString());
  }
}

QTEST_GUILESS_MAIN(MusicBrainzTestFullDate)

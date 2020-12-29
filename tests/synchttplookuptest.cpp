/*
    SPDX-FileCopyrightText: 2006 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "synchttplookuptest.h"
#include "libkcddb/client.h"
#include "libkcddb/cache.h"
#include "libkcddb/lookup.h"
#include <QTest>

void SyncHTTPLookupTest::testLookup()
{
  using namespace KCDDB;

  Client c;
  c.config().setHostname(QString::fromUtf8("gnudb.gnudb.org"));
  c.config().setPort(80);
  c.config().setCacheLookupEnabled(false);
  c.config().setFreedbLookupEnabled(true);
  c.config().setMusicBrainzLookupEnabled(false);
  c.config().setFreedbLookupTransport(Lookup::HTTP);

  TrackOffsetList list;

  list
    << 150
    << 106965
    << 127220
    << 151925
    << 176085
    << 234500;

  Result r = c.lookup(list);

  QVERIFY(r == Success);

  CDInfoList response = c.lookupResponse();

  CDInfoList::ConstIterator it;

  bool hasRunTest = false;

  for (it = response.constBegin(); it != response.constEnd(); ++it)
  {
    CDInfo i(*it);

    if (i.get(QString::fromUtf8("discid")) == QString::fromUtf8("3e0c3a05") && i.get(Category) == QString::fromUtf8("misc"))
    {
      // If revision doesn't match, test probably needs to be updated
      // See: http://www.gnudb.org/gnudb/misc/3e0c3a05 for updated data
      QCOMPARE(i.get(QString::fromUtf8("revision")).toInt(), 17);

      QCOMPARE(i.get(Artist).toString(),QString::fromUtf8("Pink Floyd"));
      QCOMPARE(i.get(Title).toString(),QString::fromUtf8("Atom Heart Mother"));
      QCOMPARE(i.get(Genre).toString(),QString::fromUtf8("Psychedelic Rock"));
      QCOMPARE(i.get(Year).toInt(),1970);
      QCOMPARE(i.track(0).get(Title).toString(),QString::fromUtf8("Atom Heart Mother: Father's Shout/Breast Milky/Mother Fore/Funky Dung/Mind Your Throats Please/Remergence"));
      QCOMPARE(i.track(1).get(Title).toString(),QString::fromUtf8("If"));
      QCOMPARE(i.track(2).get(Title).toString(),QString::fromUtf8("Summer '68"));
      QCOMPARE(i.track(3).get(Title).toString(),QString::fromUtf8("Fat Old Sun"));
      QCOMPARE(i.track(4).get(Title).toString(),QString::fromUtf8("Alan's Psychedelic Breakfast: Rise and Shine/Sunny Side Up/Morning Glory"));
      QCOMPARE(i.track(0).get(Comment).toString(),QString::fromUtf8(""));
      QCOMPARE(i.track(1).get(Comment).toString(),QString::fromUtf8(""));
      QCOMPARE(i.track(2).get(Comment).toString(),QString::fromUtf8(""));
      QCOMPARE(i.track(3).get(Comment).toString(),QString::fromUtf8(""));
      QCOMPARE(i.track(4).get(Comment).toString(),QString::fromUtf8(""));
      for (int j=0; j <= 4; j++)
      {
        QCOMPARE(i.track(j).get(Artist).toString(),QString::fromUtf8("Pink Floyd"));
      }

      hasRunTest = true;
    }
  }

  QVERIFY(hasRunTest);
}

QTEST_GUILESS_MAIN(SyncHTTPLookupTest)

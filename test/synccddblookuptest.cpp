/*
  Copyright (C) 2005 Richard Lärkäng <nouseforaname@home.se>

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

#include <QtTest/qtest.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <kapplication.h>
#include "libkcddb/client.h"
#include "libkcddb/cache.h"
#include "libkcddb/lookup.h"
#include "synccddblookuptest.h"

void SyncCDDBLookupTest::testLookup()
{
  using namespace KCDDB;

  Client c;
  c.config().setHostname("freedb.freedb.org");
  c.config().setPort(8880);
  c.config().setCachePolicy(Cache::Ignore);
  c.config().setLookupTransport(Lookup::CDDBP);

  TrackOffsetList list;

  list
    << 150
    << 106965
    << 127220
    << 151925
    << 176085
    << 234500;

  kdDebug()
    << CDDB::trackOffsetListToId(list)
    << " "
    //<< trackOffsetListToString(list)
    << endl;

  c.lookup(list);

  CDInfoList response = c.lookupResponse();

  CDInfoList::ConstIterator it;

  bool hasRunTest = false;

  for (it = response.begin(); it != response.end(); ++it)
  {
    CDInfo i(*it);

    if (i.get("discid") == "3e0c3a05" && i.get("category") == "rock")
    {
      // If any of the tests fail, check that the disc-info hasn't changed first

      QCOMPARE(i.get("artist").toString(),QString("Pink Floyd"));
      QCOMPARE(i.get("title").toString(),QString("Atom Heart Mother"));
      QCOMPARE(i.get("genre").toString(),QString("Psychedelic Rock"));
      QCOMPARE(i.get("year").toInt(),1970);
      QCOMPARE(i.trackInfoList[0].get("title").toString(),QString("Atom Heart Mother : (a) Father's Shout (b) Breast Milky (c) Mother Fore (d) Funky Dung (e) Mind Your Throats Please (f) Remergegence"));
      QCOMPARE(i.trackInfoList[1].get("title").toString(),QString("If"));
      QCOMPARE(i.trackInfoList[2].get("title").toString(),QString("Summer '68"));
      QCOMPARE(i.trackInfoList[3].get("title").toString(),QString("Fat Old Sun"));
      QCOMPARE(i.trackInfoList[4].get("title").toString(),QString("Alan's Psychedelic Breakfast : (a) Rise and Shine (b) Sunny Side Up (c) Morning Glory"));
      QCOMPARE(i.trackInfoList[0].get("extt").toString(),QString("ts Please\nf. Remergence"));
      QCOMPARE(i.trackInfoList[1].get("extt").toString(),QString("Waters"));
      QCOMPARE(i.trackInfoList[2].get("extt").toString(),QString("Wright"));
      QCOMPARE(i.trackInfoList[3].get("extt").toString(),QString("Gilmour"));
      QCOMPARE(i.trackInfoList[4].get("extt").toString(),QString("ide Up\nc. Morning Glory"));

      hasRunTest = true;
    }
  }

  QVERIFY(hasRunTest);
}

int main(int argc, char *argv[])
{
    setenv("LC_ALL", "C", 1);
    KAboutData aboutData( "qttest", "qttest", "version" );
    KCmdLineArgs::init( argc, argv, &aboutData );
    KApplication::disableAutoDcopRegistration();
    KApplication app(false);
    SyncCDDBLookupTest tc;
    return QTest::qExec( &tc, argc, argv );
}

#include "synccddblookuptest.moc"

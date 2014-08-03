/*
  Copyright (C) 2007 Richard Lärkäng <nouseforaname@home.se>

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

#include "cachetest.h"
#include "libkcddb/cache.h"

#include <qtest_kde.h>
#include "libkcddb/cdinfo.h"
#include "libkcddb/client.h"
#include "config-musicbrainz.h"

using namespace KCDDB;

void CacheTest::initTestCase()
{
  m_client = new Client;
  m_client->config().setCacheLocations(QStringList(QDir::homePath()+"/.cddbTest/"));

  // a1107d0a
  m_list
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

  m_info.set(Artist, QString().leftJustified(10, QChar(0xe4)));
  m_info.set(Title, QString().leftJustified(10, QChar(0xf6)));

  for (int i=0; i<10; i++)
  {
    m_info.track(i).set(Title, QString().leftJustified(30*(i+1), 'a'+i));
  }

}

void CacheTest::cleanupTestCase()
{
  QDir().rmdir(QDir::homePath()+"/.cddbTest/");
}

bool CacheTest::verify(const QString& source, const QString& discid, const CDInfo& info)
{
  Cache::store(m_list, info, m_client->config());

  CDInfoList results = Cache::lookup(m_list, m_client->config());
  foreach(const CDInfo &newInfo, results)
  {
    if (newInfo.get("source") == source && newInfo.get("discid").toString() == discid)
    {
      if (newInfo.get(Artist) != m_info.get(Artist))
        continue;
      if (newInfo.get(Title) != m_info.get(Title))
        continue;
      bool tracksOk = true;
      for (int i=0; i < 10; i++)
      {
        if (newInfo.track(i).get(Title) != m_info.track(i).get(Title))
        {
          tracksOk = false;
          break;
        }
      }

      if (tracksOk)
        return true;
    }
  }
  return false;
}

void CacheTest::testFreedb()
{
  CDInfo testInfo = m_info;
  testInfo.set("source", "freedb");
  testInfo.set("discid", "a1107d0a");
  testInfo.set("category", "misc");

  QVERIFY(verify("freedb", "a1107d0a", testInfo));

  QFile::remove(QDir::homePath()+"/.cddbTest/misc/a1107d0a");
  QDir().rmdir(QDir::homePath()+"/.cddbTest/misc/");
}

void CacheTest::testUser()
{
  CDInfo testInfo = m_info;
  testInfo.set("source", "user");

  QVERIFY(verify("user", "a1107d0a", testInfo));

  QFile::remove(QDir::homePath()+"/.cddbTest/user/a1107d0a");
  QDir().rmdir(QDir::homePath()+"/.cddbTest/user/");
}

void CacheTest::testMusicbrainz()
{
#ifdef HAVE_MUSICBRAINZ5
  CDInfo testInfo = m_info;
  testInfo.set("source", "musicbrainz");
  testInfo.set("discid", "wdABQ7s86gS7eVmS74CCQ6KwPUI-");

  QVERIFY(verify("musicbrainz", "wdABQ7s86gS7eVmS74CCQ6KwPUI-", testInfo));

  QFile::remove(QDir::homePath()+"/.cddbTest/musicbrainz/wdABQ7s86gS7eVmS74CCQ6KwPUI-");
  QDir().rmdir(QDir::homePath()+"/.cddbTest/musicbrainz/");
#endif
}

QTEST_KDEMAIN(CacheTest, NoGUI)

/*
    SPDX-FileCopyrightText: 2007 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "cachetest.h"
#include "libkcddb/cache.h"

#include "libkcddb/client.h"
#include "config-musicbrainz.h"
#include <QTest>

using namespace KCDDB;

void CacheTest::initTestCase()
{
  m_client = new Client;
  m_client->config().setCacheLocations(QStringList(QDir::homePath()+QString::fromUtf8("/.cddbTest/")));

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
    m_info.track(i).set(Title, QString().leftJustified(30*(i+1), QChar('a'+i)));
  }

}

void CacheTest::cleanupTestCase()
{
  QDir().rmdir(QDir::homePath()+QString::fromUtf8("/.cddbTest/"));
}

bool CacheTest::verify(const QString& source, const QString& discid, const CDInfo& info)
{
  Cache::store(m_list, info, m_client->config());

  const CDInfoList results = Cache::lookup(m_list, m_client->config());
  for (const CDInfo &newInfo : results) {
    if (newInfo.get(QString::fromUtf8("source")) == source && newInfo.get(QString::fromUtf8("discid")).toString() == discid)
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
  testInfo.set(QString::fromUtf8("source"), QString::fromUtf8("freedb"));
  testInfo.set(QString::fromUtf8("discid"), QString::fromUtf8("a1107d0a"));
  testInfo.set(QString::fromUtf8("category"), QString::fromUtf8("misc"));

  QVERIFY(verify(QString::fromUtf8("freedb"), QString::fromUtf8("a1107d0a"), testInfo));

  QFile::remove(QDir::homePath()+QString::fromUtf8("/.cddbTest/misc/a1107d0a"));
  QDir().rmdir(QDir::homePath()+QString::fromUtf8("/.cddbTest/misc/"));
}

void CacheTest::testUser()
{
  CDInfo testInfo = m_info;
  testInfo.set(QString::fromUtf8("source"), QString::fromUtf8("user"));

  QVERIFY(verify(QString::fromUtf8("user"), QString::fromUtf8("a1107d0a"), testInfo));

  QFile::remove(QDir::homePath()+QString::fromUtf8("/.cddbTest/user/a1107d0a"));
  QDir().rmdir(QDir::homePath()+QString::fromUtf8("/.cddbTest/user/"));
}

void CacheTest::testMusicbrainz()
{
#ifdef HAVE_MUSICBRAINZ5
  CDInfo testInfo = m_info;
  testInfo.set(QString::fromUtf8("source"), QString::fromUtf8("musicbrainz"));
  testInfo.set(QString::fromUtf8("discid"), QString::fromUtf8("wdABQ7s86gS7eVmS74CCQ6KwPUI-"));

  QVERIFY(verify(QString::fromUtf8("musicbrainz"), QString::fromUtf8("wdABQ7s86gS7eVmS74CCQ6KwPUI-"), testInfo));

  QFile::remove(QDir::homePath()+QString::fromUtf8("/.cddbTest/musicbrainz/wdABQ7s86gS7eVmS74CCQ6KwPUI-"));
  QDir().rmdir(QDir::homePath()+QString::fromUtf8("/.cddbTest/musicbrainz/"));
#endif
}

QTEST_GUILESS_MAIN(CacheTest)

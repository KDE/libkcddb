/*
    SPDX-FileCopyrightText: 2007 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef CACHETEST_H
#define CACHETEST_H

#include "libkcddb/cdinfo.h"
#include "libkcddb/kcddb.h"

namespace KCDDB
{
  class Client;
}

#include <QObject>

class CacheTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testFreedb();
    void testUser();
    void testMusicbrainz();
private:
    bool verify(const QString& source, const QString& discid, const KCDDB::CDInfo& info);

    KCDDB::Client* m_client;
    KCDDB::CDInfo m_info;
    KCDDB::TrackOffsetList m_list;
};

#endif

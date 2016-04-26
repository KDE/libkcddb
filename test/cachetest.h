/*
  Copyright (C) 2007 Richard Lärkäng <nouseforaname@home.se>
  Copyright (C) 2016 Angelo Scarnà <angelo.scarna@codelinsoft.it>

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
private slots:
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

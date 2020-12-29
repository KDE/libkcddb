/*
    SPDX-FileCopyrightText: 2003 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef TEST_H
#define TEST_H

#include <QEventLoop>
#include <QObject>
#include "libkcddb/client.h"
#include "libkcddb/kcddb.h"

using namespace KCDDB;

class AsyncHTTPLookupTest : public QObject
{
  Q_OBJECT
  private Q_SLOTS:
    void testLookup();
    void slotFinished(KCDDB::Result);

  private:

    QEventLoop m_eventLoop;
    KCDDB::Client * client_;
    CDInfo m_info;
    Result m_result;
};

#endif

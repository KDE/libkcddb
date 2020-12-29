/*
    SPDX-FileCopyrightText: 2004 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef TEST_H
#define TEST_H

#include <QObject>
#include "libkcddb/client.h"

using namespace KCDDB;

class QCoreApplication;

class AsyncHTTPSubmitTest : public QObject
{
  Q_OBJECT

  public:
    AsyncHTTPSubmitTest(QCoreApplication& app);

  public Q_SLOTS:

    void slotFinished(KCDDB::Result);

  private:

    QCoreApplication& app_;
    KCDDB::Client * client_;
};

#endif

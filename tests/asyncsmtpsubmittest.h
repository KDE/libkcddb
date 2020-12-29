/*
    SPDX-FileCopyrightText: 2003 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef TEST_H
#define TEST_H

#include <QObject>
#include "libkcddb/client.h"

using namespace KCDDB;

class QCoreApplication;

class AsyncSMTPSubmitTest : public QObject
{
  Q_OBJECT

  public:
    AsyncSMTPSubmitTest(QCoreApplication& app);

  public Q_SLOTS:

    void slotFinished(KCDDB::Result);

  private:

    QCoreApplication& app_;
    KCDDB::Client * client_;
};

#endif

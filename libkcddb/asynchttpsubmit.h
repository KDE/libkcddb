/*
    SPDX-FileCopyrightText: 2003 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef ASYNCHTTPSUBMIT_H
#define ASYNCHTTPSUBMIT_H

#include "httpsubmit.h"

class KJob;

namespace KCDDB
{
  class AsyncHTTPSubmit : public HTTPSubmit
  {
    Q_OBJECT
    public:
      AsyncHTTPSubmit(const QString& from, const QString& hostname, uint port);
      virtual ~AsyncHTTPSubmit();

    Q_SIGNALS:
      void finished( KCDDB::Result );
    protected:
      Result runJob(KIO::Job* job) override;
    private Q_SLOTS:
      void slotFinished(KJob*);
  } ;
}


#endif // ASYNCHTTPSUBMIT_H


/*
    SPDX-FileCopyrightText: 2003 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SYNCHTTPSUBMIT_H
#define SYNCHTTPSUBMIT_H

#include "httpsubmit.h"

namespace KCDDB
{
  class SyncHTTPSubmit : public HTTPSubmit
  {
    public:
      SyncHTTPSubmit(const QString& from, const QString& hostname, uint port);
      virtual ~SyncHTTPSubmit();

    protected:
      Result runJob(KIO::Job* job) override;
  } ;
}

#endif // SYNCHTTPSUBMIT_H

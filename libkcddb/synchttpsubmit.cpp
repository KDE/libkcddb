/*
    SPDX-FileCopyrightText: 2003 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "synchttpsubmit.h"

#include <KIO/Job>

namespace KCDDB
{
  SyncHTTPSubmit::SyncHTTPSubmit(const QString& from, const QString& hostname, uint port)
    : HTTPSubmit(from, hostname, port)
  {

  }

  SyncHTTPSubmit::~SyncHTTPSubmit()
  {

  }

  Result SyncHTTPSubmit::runJob(KIO::Job* job)
  {
    if (job->exec())
      return Success;
    else
      return UnknownError;
  }
}

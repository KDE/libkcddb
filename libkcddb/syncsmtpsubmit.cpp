/*
    SPDX-FileCopyrightText: 2003-2004 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "syncsmtpsubmit.h"

#include "cdinfo.h"

#include <KIO/Job>

namespace KCDDB
{
  SyncSMTPSubmit::SyncSMTPSubmit(const QString& hostname, uint port,
        const QString& username, const QString& from, const QString& to)
    : SMTPSubmit( hostname, port, username, from, to )
  {

  }

  SyncSMTPSubmit::~SyncSMTPSubmit()
  {

  }

  Result SyncSMTPSubmit::runJob(KIO::Job* job)
  {
    if ( job->exec() )
      return Success;

    return UnknownError;
  }
}

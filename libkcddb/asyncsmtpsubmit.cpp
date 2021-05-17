/*
    SPDX-FileCopyrightText: 2003 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "asyncsmtpsubmit.h"
#include "logging.h"

#include "cdinfo.h"

#include <KIO/Job>

namespace KCDDB
{
  AsyncSMTPSubmit::AsyncSMTPSubmit(const QString& hostname, uint port,
        const QString& username, const QString& from, const QString& to )
    : SMTPSubmit( hostname, port, username, from, to )
  {

  }

  AsyncSMTPSubmit::~AsyncSMTPSubmit()
  {

  }

  Result AsyncSMTPSubmit::runJob(KIO::Job* job)
  {
    connect( job, &KJob::result,
               this, &AsyncSMTPSubmit::slotDone );

    return Success;
  }

  void AsyncSMTPSubmit::slotDone( KJob* job )
  {
	  qCDebug(LIBKCDDB) ;
      if ( job->error()==0 )
        Q_EMIT finished( Success );
      else
        Q_EMIT finished( UnknownError );
  }
}


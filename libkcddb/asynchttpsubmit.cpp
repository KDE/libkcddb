/*
    SPDX-FileCopyrightText: 2003 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "asynchttpsubmit.h"

#include <KIO/Job>
#include <QDebug>

namespace KCDDB
{
  AsyncHTTPSubmit::AsyncHTTPSubmit(const QString& from, const QString& hostname, uint port)
    : HTTPSubmit(from, hostname, port)
  {

  }

  AsyncHTTPSubmit::~AsyncHTTPSubmit()
  {

  }

  Result AsyncHTTPSubmit::runJob(KIO::Job* job)
  {
    connect(job, &KJob::result, this, &AsyncHTTPSubmit::slotFinished);

    return Success;
  }

  void AsyncHTTPSubmit::slotFinished(KJob* job)
  {
    qDebug() << "Finished";

    if ( job->error()==0 )
      Q_EMIT finished( Success );
    else
      Q_EMIT finished( UnknownError );
  }
}

#include "moc_asynchttpsubmit.cpp"

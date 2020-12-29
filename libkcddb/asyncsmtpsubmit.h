/*
    SPDX-FileCopyrightText: 2003-2004 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef ASYNCSMTPSUBMIT_H
#define ASYNCSMTPSUBMIT_H

#include "smtpsubmit.h"

class KJob;
namespace KIO
{
  class Job;
}

namespace KCDDB
{
  class AsyncSMTPSubmit : public SMTPSubmit
  {
    Q_OBJECT

    public:
      AsyncSMTPSubmit(const QString& hostname, uint port, const QString& username,
                      const QString& from, const QString& to);
      virtual ~AsyncSMTPSubmit();

    Q_SIGNALS:
      void finished( KCDDB::Result );
    protected Q_SLOTS:
      void slotDone( KJob * );
    protected:
      Result runJob(KIO::Job* job) override;
  } ;
}

#endif // ASYNCSMTPSUBMIT_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

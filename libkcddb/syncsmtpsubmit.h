/*
    SPDX-FileCopyrightText: 2003 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SYNCSMTPSUBMIT_H
#define SYNCSMTPSUBMIT_H

#include "smtpsubmit.h"

namespace KCDDB
{
  class SyncSMTPSubmit : public SMTPSubmit
  {
    public:
      SyncSMTPSubmit(const QString& hostname, uint port, const QString& username,
                     const QString& from, const QString& to);
      virtual ~SyncSMTPSubmit();
    protected:
      Result runJob(KIO::Job* job) override;
  } ;
}

#endif // SYNCSMTPSUBMIT_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

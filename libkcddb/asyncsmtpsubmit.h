#ifndef ASYNCSMTPSUBMIT_H
#define ASYNCSMTPSUBMIT_H
/*
  Copyright (C) 2003-2004 Richard Lärkäng <nouseforaname@home.se>
  Copyright (C) 2016 Angelo Scarnà <angelo.scarna@codelinsoft.it>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

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

    signals:
      void finished( KCDDB::Result );
    protected slots:
      void slotDone( KJob * );
    protected:
      virtual Result runJob(KIO::Job* job);
  } ;
}

#endif // ASYNCSMTPSUBMIT_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

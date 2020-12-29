/*
    SPDX-FileCopyrightText: 2003 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SMTPSUBMIT_H
#define SMTPSUBMIT_H

#include "submit.h"
#include <QUrl>

namespace KCDDB
{
  class SMTPSubmit : public Submit
  {
    public:
      SMTPSubmit(const QString& hostname, uint port, const QString& username, const QString& from, const QString& to);
      virtual ~SMTPSubmit();

    protected:
      void makeDiskData( const CDInfo&, const TrackOffsetList& ) override;

      KIO::Job* createJob(const CDInfo& cdInfo) override;

      QUrl url_;
      QString from_, to_;
  } ;
}

#endif // SMTPSUBMIT_H

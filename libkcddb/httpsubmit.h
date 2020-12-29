/*
    SPDX-FileCopyrightText: 2003 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef HTTPSUBMIT_H
#define HTTPSUBMIT_H

#include "submit.h"
#include <QUrl>

namespace KCDDB
{
  class HTTPSubmit : public Submit
  {
    public:
      HTTPSubmit(const QString& from, const QString& hostname, uint port);
      virtual ~HTTPSubmit();

    protected:
      KIO::Job* createJob(const CDInfo& cdInfo) override;

      QUrl url_;
      QString from_;
  } ;
}

#endif // HTTPSUBMIT_H

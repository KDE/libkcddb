#ifndef SMTPSUBMIT_H
#define SMTPSUBMIT_H
/*
  Copyright (C) 2003 Richard Lärkäng <nouseforaname@home.se>

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

#include "submit.h"
#include <QtCore/QUrl>

namespace KCDDB
{
  class SMTPSubmit : public Submit
  {
    public:
      SMTPSubmit(const QString& hostname, uint port, const QString& username, const QString& from, const QString& to);
      virtual ~SMTPSubmit();

    protected:
      virtual void makeDiskData( const CDInfo&, const TrackOffsetList& );

      virtual KIO::Job* createJob(const CDInfo& cdInfo);

      QUrl url_;
      QString from_, to_;
  } ;
}

#endif // SMTPSUBMIT_H

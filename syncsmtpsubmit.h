#ifndef SYNCSMTPSUBMIT_H
#define SYNCSMTPSUBMIT_H
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
  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA 02111-1307, USA.
*/

#include "smtpsubmit.h"

namespace KCDDB
{
  class SyncSMTPSubmit : public SMTPSubmit
  {
    public:
      SyncSMTPSubmit(const QString&, uint, const QString&, const QString&);
      virtual ~SyncSMTPSubmit();

      virtual Result submit( const CDInfo &, const TrackOffsetList&);
    protected:
      Result parseWrite( const QString & );
  } ;
}

#endif // SYNCSMTPSUBMIT_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

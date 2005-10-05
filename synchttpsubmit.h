#ifndef SYNCHTTPSUBMIT_H
#define SYNCHTTPSUBMIT_H
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

#include "httpsubmit.h"

namespace KCDDB
{
  class SyncHTTPSubmit : public HTTPSubmit
  {
    public:
      SyncHTTPSubmit(const QString& from, const QString& hostname, uint port);
      virtual ~SyncHTTPSubmit();

    protected:
      virtual Result runJob(KIO::Job* job);
  } ;
}

#endif // SYNCHTTPSUBMIT_H

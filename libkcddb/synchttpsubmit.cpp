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

#include "synchttpsubmit.h"

#include <KIO/Job>

namespace KCDDB
{
  SyncHTTPSubmit::SyncHTTPSubmit(const QString& from, const QString& hostname, uint port)
    : HTTPSubmit(from, hostname, port)
  {

  }

  SyncHTTPSubmit::~SyncHTTPSubmit()
  {

  }

  Result SyncHTTPSubmit::runJob(KIO::Job* job)
  {
    if (job->exec())
      return Success;
    else
      return UnknownError;
  }
}

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

#include "asynchttpsubmit.h"
#include <kio/job.h>
#include <kdebug.h>

namespace KCDDB
{
  AsyncHTTPSubmit::AsyncHTTPSubmit()
    : QObject(), HTTPSubmit()
  {

  }

  AsyncHTTPSubmit::~AsyncHTTPSubmit()
  {

  }
  
  CDDB::Result AsyncHTTPSubmit::runJob(KIO::Job* job)
  {
    connect(job, SIGNAL(result(KIO::Job *)), SLOT(slotFinished(KIO::Job *)));

    return CDDB::Success;
  }
  
  void AsyncHTTPSubmit::slotFinished(KIO::Job* job)
  {
    kdDebug() << "Finished" << endl;

    if ( job->error()==0 )
      emit finished( Success );
    else
      emit finished( UnknownError );
  }
}

#include "asynchttpsubmit.moc"

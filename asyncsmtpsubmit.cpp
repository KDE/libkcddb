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

#include "asyncsmtpsubmit.h"
#include "cdinfo.h"
#include <qdatastream.h>
#include <kdebug.h>

namespace KCDDB
{
  AsyncSMTPSubmit::AsyncSMTPSubmit(const QString& hostname, uint port,
        const QString& username, const QString& from, const QString& to )
    : QObject(), SMTPSubmit( hostname, port, username, from, to )
  {

  }

  AsyncSMTPSubmit::~AsyncSMTPSubmit()
  {

  }

  CDDB::Result AsyncSMTPSubmit::submit( const CDInfo& cdInfo, const TrackOffsetList &offsetList)
  {
    makeDiskData( cdInfo, offsetList );

    if (!validCategory(cdInfo.category))
      return InvalidCategory;

    QString subject = QString("cddb %1 %2").arg(cdInfo.category, cdInfo.id);
    makeURL( subject );

    KIO::TransferJob* job = KIO::put( url_, -1, false, false, false );
    connect( job, SIGNAL( dataReq( KIO::Job*, QByteArray& ) ),
               this, SLOT(slotDataReq( KIO::Job*, QByteArray& ) ) );
    connect( job, SIGNAL( result( KIO::Job* ) ),
               this, SLOT(slotDone( KIO::Job* ) ) ); 

    sent = false;

    return Success;
  }

  void AsyncSMTPSubmit::slotDataReq( KIO::Job *, QByteArray & d )
  {
      kdDebug(60010) << k_funcinfo << endl;

      if ( !sent )
      {
        // CDDB Info should be in latin1
        d.duplicate( QCString( diskData_.utf8() ) );
	sent = true;
      }
  }

  void AsyncSMTPSubmit::slotDone( KIO::Job* job )
  {
      kdDebug(60010) << k_funcinfo << endl;
      if ( job->error()==0 )
        emit finished( Success );
      else
        emit finished( UnknownError );
  }
}

#include "asyncsmtpsubmit.moc"

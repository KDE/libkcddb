/*
  Copyright (C) 2003 Richard L�rk�ng <nouseforaname@home.se>

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
        const QString& username, const QString & password)
    : QObject(), SMTPSubmit( hostname, port, username, password )
  {

  }

  AsyncSMTPSubmit::~AsyncSMTPSubmit()
  {

  }

  CDDB::Result AsyncSMTPSubmit::submit( const CDInfo& cdInfo, const TrackOffsetList &offsetList )
  {
    makeDiskData( cdInfo, offsetList );

    QString subject = QString("cddb %1 %2").arg(validCategory( cdInfo.genre ),
        cdInfo.id);
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
      kdDebug() << k_funcinfo << endl;
 
      if ( !sent )
      {
        QDataStream s( d, IO_WriteOnly );
        s << diskData_;

        sent = true;
      }
      else
        d.resize( 0 );
  }

  void AsyncSMTPSubmit::slotDone( KIO::Job* job )
  {
      if ( job->error()==0 )
        emit finished( Success );
      else
        emit finished( UnknownError );
  }
}

#include "asyncsmtpsubmit.moc"
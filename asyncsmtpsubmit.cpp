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
    KIO::TransferJob* job = KIO::put( url_, -1, false, false, false );
    connect( job, SIGNAL( dataReq( KIO::Job*, QByteArray& ) ),
               this, SLOT(slotDataReq( KIO::Job*, QByteArray& ) ) );

    return Success;
  }

  void AsyncSMTPSubmit::slotDataReq( KIO::Job *, const QByteArray &d )
  {
      QDataStream s( d, IO_WriteOnly );
      s << diskData_;
  }
}

#include "asyncsmtpsubmit.moc"

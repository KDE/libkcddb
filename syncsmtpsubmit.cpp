/*
  Copyright (C) 2003-2004 Richard Lärkäng <nouseforaname@home.se>

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

#include "syncsmtpsubmit.h"
#include "cdinfo.h"
#include <ktempfile.h>
#include <kio/netaccess.h>

namespace KCDDB
{
  SyncSMTPSubmit::SyncSMTPSubmit(const QString& hostname, uint port,
        const QString& username, const QString& from, const QString& to)
    : SMTPSubmit( hostname, port, username, from, to )
  {

  }

  SyncSMTPSubmit::~SyncSMTPSubmit()
  {

  }

  CDDB::Result SyncSMTPSubmit::submit( const CDInfo& cdInfo, const TrackOffsetList& offsetList )
  {
    makeDiskData( cdInfo, offsetList );

    if (!validCategory(cdInfo.category))
      return InvalidCategory;

    QString subject = QString("cddb %1 %2").arg(cdInfo.category, cdInfo.id);
    makeURL(subject);

    KTempFile tmp;
    tmp.setAutoDelete(true);

    QTextStream *textStream = tmp.textStream();

    if (!textStream)
      return UnknownError;

    textStream->setEncoding(QTextStream::UnicodeUTF8);

    *textStream << diskData_ << flush;

    KURL tmpUrl;
    tmpUrl.setPath( tmp.name() );

    if ( KIO::NetAccess::file_copy( tmpUrl , url_ ) )
      return Success;

    return UnknownError;
  }
}

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

#include "smtpsubmit.h"
#include "logging.h"

#include <kio/job.h>

namespace KCDDB
{
  SMTPSubmit::SMTPSubmit(const QString& hostname, uint port, const QString& username,
        const QString& from, const QString& to)
    : Submit(), from_(from), to_(to)
  {
    url_.setProtocol(QLatin1String( "smtp" ));
    url_.setHost(hostname);
    url_.setPort(port);
    if (!username.isEmpty())
      url_.setUser(username);
    url_.setPath(QLatin1String( "/send" ));
  }

  SMTPSubmit::~SMTPSubmit()
  {

  }

  KIO::Job* SMTPSubmit::createJob(const CDInfo& cdInfo)
  {
      url_.setQuery(QString::fromLatin1("to=%1&subject=cddb %2 %3&from=%4")
      .arg(to_, cdInfo.get(Category).toString(),
        cdInfo.get(QLatin1String( "discid" )).toString(), from_));
	qCDebug(LIBKCDDB) << "Url is: " << url_.prettyUrl();

    return KIO::storedPut(diskData_.toUtf8().data(), url_, -1, KIO::HideProgressInfo);
  }

  void SMTPSubmit::makeDiskData( const CDInfo& cdInfo, const TrackOffsetList& offsetList )
  {
    diskData_ = QLatin1String( "Content-Type: text/plain; charset=\"utf-8\";\n" );

    Submit::makeDiskData(cdInfo, offsetList);
  }
}


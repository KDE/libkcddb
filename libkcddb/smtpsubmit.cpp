/*
    SPDX-FileCopyrightText: 2003 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "smtpsubmit.h"
#include "logging.h"

#include <KIO/Job>

namespace KCDDB
{
  SMTPSubmit::SMTPSubmit(const QString& hostname, uint port, const QString& username,
        const QString& from, const QString& to)
    : Submit(), from_(from), to_(to)
  {
    url_.setScheme(QLatin1String( "smtp" ));
    url_.setHost(hostname);
    url_.setPort(port);
    if (!username.isEmpty())
      url_.setUserName(username);
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
  qCDebug(LIBKCDDB) << "Url is: " << url_.toDisplayString();

    return KIO::storedPut(diskData_.toUtf8().data(), url_, -1, KIO::HideProgressInfo);
  }

  void SMTPSubmit::makeDiskData( const CDInfo& cdInfo, const TrackOffsetList& offsetList )
  {
    diskData_ = QLatin1String( "Content-Type: text/plain; charset=\"utf-8\";\n" );

    Submit::makeDiskData(cdInfo, offsetList);
  }
}


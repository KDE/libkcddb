/*
  Copyright (C) 2002 Benjamin Meyer <ben-devel@meyerhome.net>
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

#include <kdebug.h>
#include <kio/job.h>

namespace KCDDB
{
  HTTPSubmit::HTTPSubmit(const QString& from, const QString& hostname, uint port)
    : Submit(), from_(from)
  {
    url_.setProtocol("http");
    url_.setHost(hostname);
    url_.setPort(port);
    url_.setPath("/~cddb/submit.cgi");
  }

  HTTPSubmit::~HTTPSubmit()
  {

  }

  KIO::Job* HTTPSubmit::createJob(const CDInfo& cdInfo)
  {
    KIO::TransferJob* job = KIO::http_post(url_, diskData_.toUtf8().data(), KIO::HideProgressInfo);

    job->addMetaData("content-type", "Content-Type: text/plain");
    QString header;

    header += "Content-Type: text/plain\n";

    header += "Category: " + cdInfo.get(Category).toString() + '\n';
    header += "Discid: " + cdInfo.get("discid").toString() + '\n';
    header += "User-Email: " + from_ + '\n';
    // Change to sumbit
    header += "Submit-Mode: test\n";
    header += "Charset: UTF-8";

    job->addMetaData("customHTTPHeader", header);

    return job;
  }
}

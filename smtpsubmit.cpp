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

#include "smtpsubmit.h"
#include <kdebug.h>

namespace KCDDB
{
  SMTPSubmit::SMTPSubmit(const QString& hostname, uint port, const QString& username,
        const QString& from, const QString& to)
    : Submit(), from_(from), to_(to)
  {
    initURL( hostname, port, username );
  }

  SMTPSubmit::~SMTPSubmit()
  {

  }

  void SMTPSubmit::initURL(const QString& hostname, uint port, const QString& username)
  {
    url_.setProtocol("smtp");
    url_.setHost(hostname);
    url_.setPort(port);
    if (!username.isEmpty())
      url_.setUser(username);
    url_.setPath("/send");
  }

  void SMTPSubmit::makeURL( const QString& subject )
  {
    url_.setQuery(QString("to=%1&subject=%2&from=%3")
      .arg(to_, subject, from_));
    kdDebug(60010) << "Url is: " << url_.prettyURL() << endl;
  }
}


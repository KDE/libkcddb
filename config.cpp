/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002 Benjamin Meyer <ben-devel@meyerhome.net>
  Copyright (C) 2002 Nadeem Hasan <nhasan@kde.org>
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

#include <kconfig.h>
#include <qdir.h>

#include <libkcddb/config.h>

namespace KCDDB
{
    bool
  Config::operator == (const Config & other) const
  {
    return
      (
        (hostname()           == other.hostname()) &&
        (port()               == other.port()) &&
	(cachePolicy()        == other.cachePolicy()) &&
	(lookupTransport()    == other.lookupTransport()) &&
	(cacheLocations()     == other.cacheLocations()) &&
	(submitTransport()    == other.submitTransport()) &&
	(smtpHostname()       == other.smtpHostname()) &&
	(smtpPort()           == other.smtpPort()) &&
	(smtpUsername()       == other.smtpUsername())
      );
  }

    bool
  Config::operator != (const Config & other) const
  {
    return ! operator == (other);
  }

}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

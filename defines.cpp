/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002 Benjamin Meyer <ben-devel@meyerhome.net>

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

#include <kdebug.h>
#include <kextendedsocket.h>

#include <libkcddb/defines.h>

namespace KCDDB
{
    Connect::Result
  connectSocket
  (
    KExtendedSocket & socket,
    const QString   & hostname,
    uint              port
  )
  {
    socket.setHost(hostname);
    socket.setPort(port);

    int lookupReturn = socket.lookup();

    if (0 != lookupReturn)
      return Connect::HostNotFound;

    int connectReturn = socket.connect();

    if (0 != connectReturn)
      return Connect::NoResponse;

    return Connect::Success;
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

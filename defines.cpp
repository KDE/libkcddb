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

#include <libkcddb/defines.h>

namespace KCDDB
{
  QString trackOffsetListToId(const TrackOffsetList &)
  {
    // STUB
    return QString::null;
  }

  QString submitTransportToString(SubmitTransport t)
  {
    switch (t)
    {
      case LocalSubmit:
        return "LocalSubmit";
        break;

      case CDDBSubmit:
        return "CDDBSubmit";
        break;

      case SMTPSubmit:
        return "SMTPSubmit";
        break;

      default:
        return "Unknown";
        break;
    }
  }

  QString lookupTransportToString(LookupTransport t)
  {
    switch (t)
    {
      case CacheOnlyLookup:
        return "CacheOnlyLookup";
        break;

      case CDDBLookup:
        return "CDDBLookup";
        break;

      case HTTPLookup:
        return "HTTPLookup";
        break;

      case CDDBLookupIgnoreCached:
        return "CDDBLookupIgnoreCached";
        break;

      case HTTPLookupIgnoreCached:
        return "HTTPLookupIgnoreCached";
        break;

      default:
        return "Unknown";
        break;
    }
  }

  SubmitTransport stringToSubmitTransport(const QString & s)
  {
    if ("LocalSubmit" == s)
      return LocalSubmit;

    else if ("CDDBSubmit" == s)
      return CDDBSubmit;

    else if ("SMTPSubmit" == s)
      return SMTPSubmit;

    return SubmitTransport(-1);
  }

  LookupTransport stringToLookupTransport(const QString & s)
  {
    if ("CacheOnlyLookup" == s)
      return CacheOnlyLookup;

    else if ("CDDBLookup" == s)
      return CDDBLookup;

    else if ("HTTPLookup" == s)
      return HTTPLookup;

    else if ("CDDBLookupIgnoreCached" == s)
      return CDDBLookupIgnoreCached;

    else if ("HTTPLookupIgnoreCached" == s)
      return HTTPLookupIgnoreCached;

    return LookupTransport(-1);
  }

  QString errorToString(Error e)
  {
    switch (e)
    {
      case None:
        return "None";
        break;

      case HostNotFound:
        return "HostNotFound";
        break;

      case NoResponse:
        return "NoResponse";
        break;

      case NoSuchCD:
        return "NoSuchCD";
        break;

      case CannotSave:
        return "CannotSave";
        break;

      default:
        return "Unknown";
        break;
    }
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

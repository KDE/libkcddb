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
#include <libkcddb/defines.h>

namespace KCDDB
{
    QString
  trackOffsetListToId(const TrackOffsetList & list)
  {
    if (list.count() < 3)
    {
      kdDebug() << k_funcinfo << "Bogus list. Less than 3 entries." << endl;
      return QString::null;
    }

    // Taken from version by Michael Matz in kio_audiocd.

    unsigned int  id          = 0;
    int           trackCount  = list.count() - 2;

    // The last two in the list are disc begin and disc end.

    for (int i = trackCount - 1; i >= 0; i--)
    {
      int n = list[i];

      n /= 75;

      while (n > 0)
      {
        id += n % 10;
        n /= 10;
      }
    }

    unsigned int l = list[trackCount + 1];

    l -= list[trackCount];
    l /= 75;

    id = ((id % 255) << 24) | (l << 8) | trackCount;

    return QString::number(id, 16);
  }

    QString
  trackOffsetListToString(const TrackOffsetList & list)
  {
    if (list.count() < 3)
    {
      kdDebug() << k_funcinfo << "Bogus list. Less than 3 entries." << endl;
      return QString::null;
    }

    QString ret;

    // Disc start.
    ret.append(QString::number(list[list.count() - 2]));

    ret.append(" ");

    for (uint i = 0; i < list.count() - 2; i++)
    {
      ret.append(QString::number(list[i]));
      ret.append(" ");
    }

    const unsigned int cdFrames = 75;

    unsigned int discLengthInSeconds = (list[list.count() - 1]) / cdFrames;

    // Disc length in seconds.
    ret.append(QString::number(discLengthInSeconds));

    return ret;
  }

    QString
  submitTransportToString(SubmitTransport t)
  {
    switch (t)
    {
      case LocalSubmit:
        return "LocalSubmit";
        break;

      case HTTPSubmit:
        return "HTTPSubmit";
        break;

      case SMTPSubmit:
        return "SMTPSubmit";
        break;

      default:
        return "Unknown";
        break;
    }
  }

    QString
  lookupTransportToString(LookupTransport t)
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

    SubmitTransport
  stringToSubmitTransport(const QString & s)
  {
    if ("LocalSubmit" == s)
      return LocalSubmit;

    else if ("HTTPSubmit" == s)
      return HTTPSubmit;

    else if ("SMTPSubmit" == s)
      return SMTPSubmit;

    return SubmitTransport(-1);
  }

    LookupTransport
  stringToLookupTransport(const QString & s)
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

    QString
  errorToString(Error e)
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

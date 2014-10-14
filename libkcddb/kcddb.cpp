/*
  Copyright (C) 2007 Richard Lärkäng <nouseforaname@home.se>

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

#include "kcddb.h"

#include "kcddbi18n.h"

namespace KCDDB
{
    QString
  resultToString(Result r)
  {
    switch (r)
    {
      case Success:
        return i18n("Success");
        break;

      case ServerError:
        return i18n("Server error");
        break;

      case HostNotFound:
        return i18n("Host not found");
        break;

      case NoResponse:
        return i18n("No response");
        break;

      case NoRecordFound:
        return i18n("No record found");
        break;

      case MultipleRecordFound:
        return i18n("Multiple records found");
        break;

      case CannotSave:
        return i18n("Cannot save");
        break;

      case InvalidCategory:
        return i18n("Invalid category");
        break;

      default:
        return i18n("Unknown error");
        break;
    }
  }
}

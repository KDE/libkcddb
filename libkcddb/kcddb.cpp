/*
    SPDX-FileCopyrightText: 2007 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
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

/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>
    SPDX-FileCopyrightText: 2002 Nadeem Hasan <nhasan@kde.org>
    SPDX-FileCopyrightText: 2003 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCDDB_CONFIG_H
#define KCDDB_CONFIG_H

#include <kcddb_export.h>
#include "configbase.h"

namespace KCDDB
{
  class KCDDB_EXPORT Config : public ConfigBase
  {
    public:
      Config();

      void reparse();
    private:
      void loadEmailSettings();
  };
}

#endif // KCDDB_CONFIG_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

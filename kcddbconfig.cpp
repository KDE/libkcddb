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
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#include <kemailsettings.h>

#include <libkcddb/kcddbconfig.h>

namespace KCDDB
{
  Config::Config()
    : ConfigBase()
  {
    loadEmailSettings();
  }

  void Config::loadEmailSettings()
  {
    KEMailSettings kes;
    kes.setProfile( kes.defaultProfileName() );

    static_cast<KConfigSkeleton::ItemString *>(findItem("emailAddress"))
      ->setDefaultValue(kes.getSetting( KEMailSettings::EmailAddress ));
    static_cast<KConfigSkeleton::ItemString *>(findItem("replyTo"))
      ->setDefaultValue(kes.getSetting( KEMailSettings::ReplyToAddress ));
    static_cast<KConfigSkeleton::ItemString *>(findItem("smtpHostname"))
      ->setDefaultValue(kes.getSetting( KEMailSettings::OutServer ));
  }

  void Config::reparse()
  {
    loadEmailSettings();

    readConfig();
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

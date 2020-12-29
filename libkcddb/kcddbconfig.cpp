/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>
    SPDX-FileCopyrightText: 2002 Nadeem Hasan <nhasan@kde.org>
    SPDX-FileCopyrightText: 2003 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include <kcddbconfig.h>

#include <KConfigCore/KEMailSettings>

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

    static_cast<KConfigSkeleton::ItemString *>(findItem(QLatin1String( "emailAddress" )))
      ->setDefaultValue(kes.getSetting( KEMailSettings::EmailAddress ));
    static_cast<KConfigSkeleton::ItemString *>(findItem(QLatin1String( "replyTo" )))
      ->setDefaultValue(kes.getSetting( KEMailSettings::ReplyToAddress ));
    static_cast<KConfigSkeleton::ItemString *>(findItem(QLatin1String( "smtpHostname" )))
      ->setDefaultValue(kes.getSetting( KEMailSettings::OutServer ));
  }

  void Config::reparse()
  {
    loadEmailSettings();

    load();
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

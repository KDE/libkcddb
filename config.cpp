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
#include <kemailsettings.h>
#include <klocale.h>

#include <libkcddb/config.h>

namespace KCDDB
{
  Config::Config() : ConfigBase()
  {
    loadGlobalSettings();
  }

  QString Config::globalEmail() const
  {
    return _senderAddress;
  }

  QString Config::globalReplyTo() const
  {
    return _senderReplyTo;
  }

  QString Config::globalSmtpHost() const
  {
    return _senderHost;
  }

  QString Config::smtpHostname() const
  {
    if (useGlobalEmail())
      return globalSmtpHost();
    else
      return ownSmtpHost();
  }

  QString Config::emailAddress() const
  {
    if (useGlobalEmail())
      return globalEmail();
    else
      return ownEmail();
  }

  QString Config::replyTo() const
  {
    if (useGlobalEmail())
      return globalReplyTo();
    else
      return ownReplyTo();
  }

  void Config::loadGlobalSettings()
  {
    KEMailSettings kes;
    kes.setProfile( kes.defaultProfileName() );
    _senderAddress = kes.getSetting( KEMailSettings::EmailAddress );
    _senderReplyTo = kes.getSetting( KEMailSettings::ReplyToAddress );
    _senderHost = kes.getSetting( KEMailSettings::OutServer );
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

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

#include <kconfig.h>

#include <libkcddb/config.h>

namespace KCDDB
{
  // Defaults.

  static const Submit::Transport  defaultSubmitTransport = Submit::CDDB;
  static const Lookup::Transport  defaultLookupTransport = Lookup::CDDB;

  static const char * const defaultHostname       = "freedb.freedb.org";
  static const unsigned int defaultPort           = 8880;
  static const char * const defaultUser           = "libkcddb-user";
  static const char * const defaultClientName     = "libkcddb";
  static const char * const defaultClientVersion  = "0.1";
  static const char * const defaultEmailAddress   = "freedb-submit@freedb.org";
  static const bool         defaultSubmissionsEnabled = true;

  Config::Config()
    : hostname_             (defaultHostname),
      port_                 (defaultPort),
      user_                 (defaultUser),
      clientName_           (defaultClientName),
      clientVersion_        (defaultClientVersion),
      submitTransport_      (defaultSubmitTransport),
      lookupTransport_      (defaultLookupTransport),
      emailAddress_         (defaultEmailAddress),
      submissionsEnabled_   (defaultSubmissionsEnabled)
  {
  }

    bool
  Config::operator == (const Config & other) const
  {
    return
      (
        (hostname_            == other.hostname_)
        &&
        (port_                == other.port_)
        &&
        (user_                == other.user_)
        &&
        (clientName_          == other.clientName_)
        &&
        (clientVersion_       == other.clientVersion_)
        &&
        (submitTransport_     == other.submitTransport_)
        &&
        (lookupTransport_     == other.lookupTransport_)
        &&
        (emailAddress_        == other.emailAddress_)
        &&
        (submissionsEnabled_  == other.submissionsEnabled_)
      );
  }

    bool
  Config::operator != (const Config & other) const
  {
    return ! operator == (other);
  }

    void
  Config::load(const QString & filename)
  {
    KConfig c(filename);

    hostname_ = c.readEntry(hostnameKey(), defaultHostname);

    port_ = c.readUnsignedNumEntry(portKey(), defaultPort);

    user_ = c.readEntry(userKey(), defaultUser);

    clientName_ = c.readEntry(clientNameKey(), defaultClientName);

    clientVersion_ = c.readEntry(clientVersionKey(), defaultClientVersion);

    submitTransport_ =
      Submit::stringToTransport
      (
        c.readEntry
        (
          submitTransportKey(),
          Submit::transportToString(defaultSubmitTransport)
        )
      );

    lookupTransport_ =
      Lookup::stringToTransport
      (
        c.readEntry
        (
          lookupTransportKey(),
          Lookup::transportToString(defaultLookupTransport)
        )
      );

    emailAddress_ = c.readEntry(emailAddressKey(), defaultEmailAddress);

    submissionsEnabled_ =
      c.readBoolEntry(submissionsEnabledKey(), defaultSubmissionsEnabled);
  }

    void
  Config::save(const QString & filename)
  {
    KConfig c(filename);

    c.writeEntry(hostnameKey(), hostname_);

    c.writeEntry(portKey(), port_);

    c.writeEntry(userKey(), user_);

    c.writeEntry(clientNameKey(), clientName_);

    c.readEntry(clientVersionKey(), clientVersion_);

    c.writeEntry
      (
        submitTransportKey(),
        Submit::transportToString(submitTransport_)
      );

    c.writeEntry
      (
        lookupTransportKey(),
        Lookup::transportToString(lookupTransport_)
      );

    c.writeEntry(emailAddressKey(), emailAddress_);

    c.writeEntry(submissionsEnabledKey(), submissionsEnabled_);

    c.sync();
  }

  // Get methods. Bloody C++.

    QString
  Config::hostname() const
  {
    return hostname_;
  }

    uint
  Config::port() const
  {
    return port_;
  }

    QString
  Config::user() const
  {
    return user_;
  }

    QString
  Config::clientName() const
  {
    return clientName_;
  }

    QString
  Config::clientVersion() const
  {
    return clientVersion_;
  }

    Submit::Transport
  Config::submitTransport() const
  {
    return submitTransport_;
  }

    Lookup::Transport
  Config::lookupTransport() const
  {
    return lookupTransport_;
  }

    QString
  Config::emailAddress() const
  {
    return emailAddress_;
  }

    bool
  Config::submissionsEnabled() const
  {
    return submissionsEnabled_;
  }

  // Set methods. Bloody C++.

    void
  Config::setHostname(const QString & s)
  {
    hostname_ = s;
  }

    void
  Config::setPort(uint i)
  {
    port_ = i;
  }

    void
  Config::setUser(const QString & s)
  {
    user_ = s;
  }

    void
  Config::setClientName(const QString & s)
  {
    clientName_ = s;
  }

    void
  Config::setClientVersion(const QString & s)
  {
    clientVersion_ = s;
  }

    void
  Config::setSubmitTransport(Submit::Transport t)
  {
    submitTransport_ = t;
  }

    void
  Config::setLookupTransport(Lookup::Transport t)
  {
    lookupTransport_ = t;
  }

    void
  Config::setEmailAddress(const QString & s)
  {
    emailAddress_ = s;
  }

    void
  Config::setSubmissionsEnabled(bool b)
  {
    submissionsEnabled_ = b;
  }

  // Config keys.

    QString
  Config::hostnameKey()
  {
    return "Host";
  }

    QString
  Config::portKey()
  {
    return "Port";
  }

    QString
  Config::userKey()
  {
    return "User";
  }

    QString
  Config::clientNameKey()
  {
    return "ClientName";
  }

    QString
  Config::clientVersionKey()
  {
    return "ClientVersion";
  }

    QString
  Config::submitTransportKey()
  {
    return "SubmitTransport";
  }

    QString
  Config::lookupTransportKey()
  {
    return "LookupTransport";
  }

    QString
  Config::emailAddressKey()
  {
    return "EmailAddress";
  }

    QString
  Config::submissionsEnabledKey()
  {
    return "SubmissionsEnabled";
  }

    void
  Config::setCachePolicy(Cache::Policy p)
  {
    cachePolicy_ = p;
  }

    Cache::Policy
  Config::cachePolicy() const
  {
    return cachePolicy_;
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

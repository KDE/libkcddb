/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002 Benjamin Meyer <ben-devel@meyerhome.net>
  Copyright (C) 2002 Nadeem Hasan <nhasan@kde.org>

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

  static const CDDB::Transport  defaultSubmitTransport = CDDB::CDDBP;
  static const CDDB::Transport  defaultLookupTransport = CDDB::CDDBP;

  static const char * const  defaultHostname     = "freedb.freedb.org";
  static const unsigned int  defaultPort         = 80;
  static const char * const  defaultEmailAddress = "freedb-submit@freedb.org";
  static const bool          defaultSubmissionsEnabled = true;
  static const Cache::Policy defaultCachePolicy  = Cache::Use;

  Config::Config()
    : hostname_             (defaultHostname),
      port_                 (defaultPort),
      submitTransport_      (defaultSubmitTransport),
      lookupTransport_      (defaultLookupTransport),
      emailAddress_         (defaultEmailAddress),
      submissionsEnabled_   (defaultSubmissionsEnabled),
      cachePolicy_          (defaultCachePolicy)
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
        &&
        (cachePolicy_         == other.cachePolicy_)
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

    submitTransport_ =
      CDDB::stringToTransport
      (
        c.readEntry
        (
          submitTransportKey(),
          CDDB::transportToString(defaultSubmitTransport)
        )
      );

    lookupTransport_ =
      CDDB::stringToTransport
      (
        c.readEntry
        (
          lookupTransportKey(),
          CDDB::transportToString(defaultLookupTransport)
        )
      );

    emailAddress_ = c.readEntry(emailAddressKey(), defaultEmailAddress);

    submissionsEnabled_ =
      c.readBoolEntry(submissionsEnabledKey(), defaultSubmissionsEnabled);

    QString p = c.readEntry(cachePolicyKey(), QString::null);
    if (p == "cacheOnly")
        cachePolicy_ = Cache::Only;
    else if (p == "cacheAndRemote")
        cachePolicy_ = Cache::Use;
    else if (p == "remoteOnly")
        cachePolicy_ = Cache::Ignore;
    else
        cachePolicy_ = defaultCachePolicy;
  }

    void
  Config::save(const QString & filename)
  {
    KConfig c(filename);

    c.writeEntry(hostnameKey(), hostname_);
    c.writeEntry(portKey(), port_);

    c.writeEntry
      (
        submitTransportKey(),
        CDDB::transportToString(submitTransport_)
      );

    c.writeEntry
      (
        lookupTransportKey(),
        CDDB::transportToString(lookupTransport_)
      );

    c.writeEntry(emailAddressKey(), emailAddress_);
    c.writeEntry(submissionsEnabledKey(), submissionsEnabled_);
		if (cachePolicy_ == Cache::Only)
				c.writeEntry(cachePolicyKey(), "cacheOnly");
		else if (cachePolicy_ == Cache::Use)
				c.writeEntry(cachePolicyKey(), "cacheAndRemote");
	  else if (cachePolicy_ == Cache::Ignore)
		    c.writeEntry(cachePolicyKey(), "remoteOnly");

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

    CDDB::Transport
  Config::submitTransport() const
  {
    return submitTransport_;
  }

    CDDB::Transport
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

    Cache::Policy
  Config::cachePolicy() const
  {
    return cachePolicy_;
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
  Config::setSubmitTransport(CDDB::Transport t)
  {
    submitTransport_ = t;
  }

    void
  Config::setLookupTransport(CDDB::Transport t)
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

    void
  Config::setCachePolicy(Cache::Policy p)
  {
    cachePolicy_ = p;
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

    QString
  Config::cachePolicyKey()
  {
    return "CachePolicy";
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

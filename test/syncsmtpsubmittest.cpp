/*
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
  the Free Software Foundation, Inc., 51 Franklin Steet, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#include <kdebug.h>
#include <kapplication.h>
#include <kcmdlineargs.h>

#include "libkcddb/client.h"
#include "libkcddb/config.h"
#include "libkcddb/submit.h"

  int
main(int argc, char ** argv)
{
  KCmdLineArgs::init(argc, argv, "libkcddb_test", "", "", "");

  KApplication app(false /* No styles */);

  using namespace KCDDB;

  TrackOffsetList list;

  list
    << 150      // First track start.
    << 2592
    << 35472
    << 47891
    << 123310
    << 150       // Disc start.
    << 133125;  // Disc end.

  CDInfo cdInfo;

  cdInfo.set("discid", "3606ed05");
  cdInfo.revision = 4;
  cdInfo.set("title", "Bamse och Bronto");
  cdInfo.set("artist", "Musiksage");
  cdInfo.set("year", 2001);
  cdInfo.set("category", "misc");
  cdInfo.set("genre", "Barnsaga");
  cdInfo.set("extd", QString::fromUtf8("Berättare: Olof Thunberg"));

  TrackInfo info;
  info.set("title", "Bamses signaturmelodi");
  cdInfo.trackInfoList.append(info);
  info.set("title","*");
  cdInfo.trackInfoList.append(info);
  info.set("title", "Brummavisan");
  cdInfo.trackInfoList.append(info);
  info.set("title", "*");
  cdInfo.trackInfoList.append(info);
  info.set("title", QString::fromUtf8("Jätteödlan Bronto"));
  cdInfo.trackInfoList.append(info);

  Client c;
  c.config().setSubmitTransport(Submit::SMTP);
  c.config().setSubmitAddress("test-submit@freedb.org");

  c.setBlockingMode( true );

  CDDB::Result r = c.submit(cdInfo, list);

  kdDebug() << "Result: " << CDDB::resultToString(r) << endl;
}


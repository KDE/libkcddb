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
  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA 02111-1307, USA.
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

  // a1107d0a - Kruder & Dorfmeister - The K&D Sessions - Disc One.
  list
    << 150      // First track start.
    << 29462
    << 66983
    << 96785
    << 135628
    << 168676
    << 194147
    << 222158
    << 247076
    << 278203   // Last track start.
    << 10       // Disc start.
    << 316732;  // Disc end.

/*  CDInfo cdInfo;

  cdInfo.title  = "The K&D Sessions";
  cdInfo.artist = "Kruder & Dorfmeister";
  cdInfo.year   = 2000;
  cdInfo.genre  = "misc";*/

  Client c;
  c.config().setSubmitTransport(Submit::SMTP);
  c.config().setSubmitAddress("test-submit@freedb.org");

  c.setBlockingMode( true );

  c.lookup( list );

  CDDB::Result r = c.submit(c.lookupResponse().first(), list);

  kdDebug() << "Result: " << CDDB::resultToString(r) << endl;
}


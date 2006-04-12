/*
  Copyright (C) 2004-2005 Richard Lärkäng <nouseforaname@home.se>

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

#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kdebug.h>

#include "libkcddb/client.h"
#include "libkcddb/cache.h"
#include "libkcddb/lookup.h"

  int
main(int argc, char ** argv)
{
  KCmdLineArgs::init(argc, argv, "libkcddb_test", "", "", "");

  KApplication app;

  using namespace KCDDB;

  Client c;

  TrackOffsetList list;

  list
  << 150
  << 61408
  << 177675;

  kDebug() << "Stuff to send to server:" << endl;

  kDebug()
    << CDDB::trackOffsetListToId(list)
    << " "
    //<< trackOffsetListToString(list)
    << endl;

  CDDB::Result r = c.lookup(list);

  kDebug() << "Client::lookup gave : " << CDDB::resultToString(r) << endl;

  CDInfoList response = c.lookupResponse();

  kDebug() << "Client::lookup returned : " << response.count() << " entries"
    << endl;

  CDInfo i( c.lookupResponse().first() );

  kDebug() << "First CDInfo had title: " << i.get("title").toString() << endl;
  
  return 0;
}

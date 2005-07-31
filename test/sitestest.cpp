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

#include "libkcddb/sites.h"
#include <qlist.h>

  int
main(int argc, char ** argv)
{
  KCmdLineArgs::init(argc, argv, "libkcddb_test", "", "", "");

  KApplication app(false /* No styles */);

  using namespace KCDDB;

  Sites s;

  kdDebug() << "Sites: " << endl;

  QList<Mirror> sites = s.siteList();
  for (QList<Mirror>::Iterator it = sites.begin(); it != sites.end(); ++it)
    if ((*it).transport == Lookup::CDDBP)
      kdDebug() << (*it).address << " CDDBP " << (*it).port << " " << (*it).description << endl;
    else
      kdDebug() << (*it).address << " HTTP " << (*it).port << " " << (*it).description << endl;

  return 0;
}

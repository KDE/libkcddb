#include <iostream>

#include <kapplication.h>
#include <kcmdlineargs.h>

#include <libkcddb/client.h>

  int
main(int argc, char ** argv)
{
  KCmdLineArgs::init(argc, argv, "libkcddb_test", "", "");

  KApplication app(false /* No styles */, false /* No GUI */);

  using namespace KCDDB;

  Client c;

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

  std::cerr << "Stuff to send to server:" << std::endl;

  std::cerr
    << trackOffsetListToId(list)
    << " "
    << trackOffsetListToString(list)
    << std::endl;

  Error e = c.lookup(list);

  std::cerr << errorToString(e) << std::endl;

  return 0;
}

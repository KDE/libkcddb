#include <kdebug.h>
#include <kapplication.h>
#include <kcmdlineargs.h>

#include <libkcddb/client.h>

  int
main(int argc, char ** argv)
{
  KCmdLineArgs::init(argc, argv, "libkcddb_test", "", "");

  KApplication app(false /* No styles */, false /* No GUI */);

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

  CDInfo cdInfo;

  cdInfo.title  = "The K&D Sessions";
  cdInfo.artist = "Kruder & Dorfmeister";
  cdInfo.year   = 2000;
  cdInfo.genre  = "misc";

  Client c;

  KCDDB::Error error = c.submit(cdInfo);

  kdDebug() << "Result: " << KCDDB::errorToString(error) << endl;
}


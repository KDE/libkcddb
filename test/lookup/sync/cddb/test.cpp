#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kdebug.h>

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

  kdDebug() << "Stuff to send to server:" << endl;

  kdDebug()
    << trackOffsetListToId(list)
    << " "
    << trackOffsetListToString(list)
    << endl;

  Error e = c.lookup(list);

  kdDebug() << "Client::lookup returned error: " << errorToString(e)
    << endl;

  QValueList<CDInfo> response = c.lookupResponse();

  kdDebug() << "Client::lookup returned : " << response.count() << " entries"
    << endl;

  QValueList<CDInfo>::ConstIterator it;

  for (it = response.begin(); it != response.end(); ++it)
  {
    CDInfo i(*it);

    kdDebug() << "Disc title: " << i.title << endl;
  }
  return 0;
}

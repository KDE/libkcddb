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

  Config config;
  config.setHostname("localhost");
  config.setPort(8880);
  config.setCachePolicy(KCDDB::Cache::Ignore);
  config.setLookupTransport(KCDDB::Lookup::CDDB);

  Client c(config);

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

  Lookup::Result r = c.lookup(list);

  kdDebug() << "Client::lookup gave : " << Lookup::resultToString(r) << endl;

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

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
  config.setHostname("freedb.freedb.org");
  config.setPort(80);
  config.setCachePolicy(KCDDB::Cache::Use);
  config.setLookupTransport(KCDDB::Lookup::HTTP);

  Client c(config);

  TrackOffsetList list;

  // a1107d0a - Kruder & Dorfmeister - The K&D Sessions - Disc One.
//  list
//    << 150      // First track start.
//    << 29462
//    << 66983
//    << 96785
//    << 135628
//    << 168676
//    << 194147
//    << 222158
//    << 247076
//    << 278203   // Last track start.
//    << 10       // Disc start.
//    << 316732;  // Disc end.
  list
    << 150
    << 106965
    << 127220
    << 151925
    << 176085
    << 5
    << 234500;

  kdDebug() << "Stuff to send to server:" << endl;

  kdDebug()
    << Lookup::trackOffsetListToId(list)
    << " "
    //<< trackOffsetListToString(list)
    << endl;

  Lookup::Result r = c.lookup(list);

  kdDebug() << "Client::lookup gave : " << Lookup::resultToString(r) << endl;

  CDInfoList response = c.lookupResponse();

  kdDebug() << "Client::lookup returned : " << response.count() << " entries"
    << endl;

  CDInfoList::ConstIterator it;

  for (it = response.begin(); it != response.end(); ++it)
  {
    CDInfo i(*it);

    kdDebug() << "Disc title: " << i.title << endl;
    kdDebug() << "Total tracks: " << i.trackInfoList.count() << endl;
  }
  return 0;
}
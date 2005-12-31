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

  KApplication app(false /* No styles */);

  using namespace KCDDB;

  Client c;
  c.config().setHostname("freedb.freedb.org");
  c.config().setPort(80);
  c.config().setCachePolicy(Cache::Ignore);
  c.config().setLookupTransport(Lookup::HTTP);


  TrackOffsetList list;

  list
    << 150
    << 106965
    << 127220
    << 151925
    << 176085
    << 234500;

  kdDebug() << "Stuff to send to server:" << endl;

  kdDebug()
    << CDDB::trackOffsetListToId(list)
    << " "
    //<< trackOffsetListToString(list)
    << endl;

  CDDB::Result r = c.lookup(list);

  kdDebug() << "Client::lookup gave : " << CDDB::resultToString(r) << endl;

  CDInfoList response = c.lookupResponse();

  kdDebug() << "Client::lookup returned : " << response.count() << " entries"
    << endl;

  CDInfoList::ConstIterator it;

  for (it = response.begin(); it != response.end(); ++it)
  {
    CDInfo i(*it);

    kdDebug() << "Disc title: " << i.get("title").toString() << endl;
    kdDebug() << "Total tracks: " << i.numberOfTracks() << endl;
    kdDebug() << "Disc revision: `" << i.get("revision").toInt() << "'" << endl;
  }
  
  return 0;
}

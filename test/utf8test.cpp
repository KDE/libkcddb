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
  << 0
  << 177675;

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

  CDInfo i( c.bestLookupResponse() );

  kdDebug() << "Best CDInfo had title: " << i.title << endl;
  
  return 0;
}

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

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

  KApplication app(false);

  using namespace KCDDB;

  Client c;
  c.config().setCachePolicy(Cache::Ignore);
  c.config().setLookupTransport(Lookup::MusicBrainz);

  TrackOffsetList list;

  //The Liptones / The Latest News
  list
    << 150
    << 9219
    << 20386
    << 34134
    << 51437
    << 68809
    << 86591
    << 106288
    << 115568
    << 133312
    << 155593
    << 169832
    << 179626
    << 194958
    << 75
    << 212506;


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
    kdDebug() << "Total tracks: " << i.trackInfoList.count() << endl;
    kdDebug() << "Disc revision: `" << i.revision << "'" << endl;
  }

  CDInfo i( c.bestLookupResponse() );

  kdDebug() << "Best CDInfo had title: " << i.get("title").toString() << endl;
  kdDebug() << "and revision: " << i.revision << endl;
  for (TrackInfoList::Iterator it = i.trackInfoList.begin(); it != i.trackInfoList.end(); ++it)
  {
    kdDebug() << "Track title: " << (*it).get("title").toString() << endl;
  }

  return 0;
}

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
    kdDebug() << "Total tracks: " << i.numberOfTracks() << endl;
    kdDebug() << "Disc revision: `" << i.get("revision").toInt() << "'" << endl;
  }

  CDInfo info( c.lookupResponse().first() );

  kdDebug() << "First CDInfo had title: " << info.get("title").toString() << endl;
  kdDebug() << "and revision: " << info.get("revision") << endl;
  for (int i=0; i < info.numberOfTracks(); i++)
  {
    kdDebug() << "  Track: `" << info.track(i).get("title").toString() << "'" << endl;
  }

  return 0;
}

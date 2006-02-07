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

  CDInfoList::ConstIterator it;

  for (it = response.begin(); it != response.end(); ++it)
  {
    CDInfo i(*it);

    kDebug() << "Disc title: " << i.get("title").toString() << endl;
    kDebug() << "Total tracks: " << i.numberOfTracks() << endl;
    kDebug() << "Disc revision: `" << i.get("revision").toInt() << "'" << endl;
  }

  CDInfo info( c.lookupResponse().first() );

  kDebug() << "First CDInfo had title: " << info.get("title").toString() << endl;
  kDebug() << "and revision: " << info.get("revision") << endl;
  for (int i=0; i < info.numberOfTracks(); i++)
  {
    kDebug() << "  Track: `" << info.track(i).get("title").toString() << "'" << endl;
  }

  return 0;
}

#include <kdebug.h>
#include <kapplication.h>
#include <kcmdlineargs.h>

#include "test.h"

AsyncHTTPLookupTest::AsyncHTTPLookupTest()
  : QObject()
{
  using namespace KCDDB;

  Config config;

  config.setHostname("freedb.freedb.org");
  config.setPort(8880);
  config.setCachePolicy(Cache::Ignore);
  config.setLookupTransport(Lookup::HTTP);

  client_ = new AsyncClient(config, this);

  connect
    (
      client_,
      SIGNAL(result(Lookup::Result, const QValueList<CDInfo> &)),
      SLOT(slotResult(Lookup::Result, const QValueList<CDInfo> &))
    );

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

  client_->lookup(list);
}

  void
AsyncHTTPLookupTest::slotResult(KCDDB::Lookup::Result r,const QValueList<CDInfo> & l)
{
  kdDebug() << "AsyncHTTPLookupTest::slotResult: Got " << KCDDB::Lookup::resultToString(r) << endl;
  kdDebug() << "AsyncHTTPLookupTest::slotResult: Item count: " <<  l.count() << endl;

  for (QValueList<CDInfo>::ConstIterator it(l.begin()); it != l.end(); ++it)
  {
    CDInfo i(*it);

    kdDebug() << "Disc artist: `" << i.artist << "'" << endl;
    kdDebug() << "Disc title: `" << i.title << "'" << endl;
  }

  if (!l.isEmpty())
  {
    kdDebug() << "---------------------------------------" << endl;
    kdDebug() << "Showing first item" << endl;

    CDInfo i(l.first());

    kdDebug() << "Disc artist: `" << i.artist << "'" << endl;
    kdDebug() << "Disc title: `" << i.title << "'" << endl;
    kdDebug() << "Disc genre: `" << i.genre << "'" << endl;
    kdDebug() << "Disc year: `" << i.year << "'" << endl;
    kdDebug() << "Disc length: `" << i.length << "'" << endl;
    kdDebug() << "Disc id: `" << i.id << "'" << endl;
    kdDebug() << "Tracks........" << endl;

    for (TrackInfoList::ConstIterator it(i.trackInfoList.begin()); it != i.trackInfoList.end(); ++it)
    {
      kdDebug() << "  Track: `" << (*it).title << "'" << endl;
    }
    kdDebug() << "---------------------------------------" << endl;
  }

  kapp->quit();
}

int main(int argc, char ** argv)
{
  KCmdLineArgs::init(argc, argv, "libkcddb_test", "", "");

  KApplication app(false /* No styles */, false /* No GUI */);

  new AsyncHTTPLookupTest;

  return app.exec();
}

#include <kdebug.h>
#include <kapplication.h>
#include <kcmdlineargs.h>

#include "asynchttplookuptest.h"
#include "libkcddb/cache.h"
#include "libkcddb/lookup.h"

AsyncHTTPLookupTest::AsyncHTTPLookupTest()
  : QObject()
{
  using namespace KCDDB;

  client_ = new Client;
  client_->config().setHostname("freedb.freedb.org");
  client_->config().setPort(80);
  client_->config().setCachePolicy(Cache::Ignore);
  client_->config().setLookupTransport(Lookup::HTTP);
  client_->setBlockingMode( false );

  connect
    (
      client_,
      SIGNAL(finished(CDDB::Result)),
      SLOT(slotFinished(CDDB::Result))
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
    << 316732;  // Disc end.

  client_->lookup(list);
}

AsyncHTTPLookupTest::~AsyncHTTPLookupTest()
{
  delete client_;
}

  void
AsyncHTTPLookupTest::slotFinished(CDDB::Result r)
{
  kdDebug() << "AsyncHTTPLookupTest::slotFinished: Got " << KCDDB::CDDB::resultToString(r) << endl;

  CDInfoList l = client_->lookupResponse();

  kdDebug() << "AsyncHTTPLookupTest::slotFinished: Item count: " <<  l.count() << endl;

  for (CDInfoList::ConstIterator it(l.begin()); it != l.end(); ++it)
  {
    CDInfo i(*it);

    kdDebug() << "Disc artist: `" << i.get("artist").toString() << "'" << endl;
    kdDebug() << "Disc title: `" << i.get("title").toString() << "'" << endl;
    kdDebug() << "Disc revision: `" << i.get("revision") << "'" << endl;
  }

  if (!l.isEmpty())
  {
    kdDebug() << "---------------------------------------" << endl;
    kdDebug() << "Showing first item" << endl;

    CDInfo info(l.first());

    kdDebug() << "Disc artist: `" << info.get("artist").toString() << "'" << endl;
    kdDebug() << "Disc title: `" << info.get("title").toString() << "'" << endl;
    kdDebug() << "Disc genre: `" << info.get("genre").toString() << "'" << endl;
    kdDebug() << "Disc year: `" << info.get("year").toString() << "'" << endl;
    kdDebug() << "Disc length: `" << info.get("length").toString() << "'" << endl;
    kdDebug() << "Disc id: `" << info.get("discid").toString() << "'" << endl;
    kdDebug() << "Tracks........" << endl;

    for (int i=0; i < info.numberOfTracks(); i++)
    {
      kdDebug() << "  Track: `" << info.track(i).get("title").toString() << "'" << endl;
    }
    kdDebug() << "---------------------------------------" << endl;
  }

  kapp->quit();
}

int main(int argc, char ** argv)
{
  KCmdLineArgs::init(argc, argv, "libkcddb_test", "", "", "");

  KApplication app(false);

  AsyncHTTPLookupTest a;

  return app.exec();
}

#include "asynchttplookuptest.moc"

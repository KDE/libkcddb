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
    << 10       // Disc start.
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

    kdDebug() << "Disc artist: `" << i.artist << "'" << endl;
    kdDebug() << "Disc title: `" << i.title << "'" << endl;
    kdDebug() << "Disc revision: `" << i.revision << "'" << endl;
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
  CDInfo i( client_->bestLookupResponse() );

  kdDebug() << "Best CDInfo had title: " << i.title << endl;
  kdDebug() << "and revision: " << i.revision << endl;

  kapp->quit();
}

int main(int argc, char ** argv)
{
  KCmdLineArgs::init(argc, argv, "libkcddb_test", "", "", "");

  KApplication app(false /* No styles */, false /* No GUI */);

  AsyncHTTPLookupTest a;

  return app.exec();
}

#include "asynchttplookuptest.moc"

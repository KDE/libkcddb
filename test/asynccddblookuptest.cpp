#include <kdebug.h>
#include <kapplication.h>
#include <kcmdlineargs.h>

#include "asynccddblookuptest.h"

#include "libkcddb/cache.h"
#include "libkcddb/lookup.h"

AsyncCDDBLookupTest::AsyncCDDBLookupTest()
  : QObject()
{
  using namespace KCDDB;

  client_ = new Client;
  client_->config().setHostname("freedb.freedb.org");
  client_->config().setPort(8880);
  client_->config().setCachePolicy(Cache::Ignore);
  client_->config().setLookupTransport(Lookup::CDDBP);
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

AsyncCDDBLookupTest::~AsyncCDDBLookupTest()
{
  delete client_;
}

  void
AsyncCDDBLookupTest::slotFinished(CDDB::Result r)
{
  kdDebug() << "AsyncCDDBLookupTest::slotResult: Got " << KCDDB::CDDB::resultToString(r) << endl;

  CDInfoList l = client_->lookupResponse();

  kdDebug() << "AsyncCDDBLookupTest::slotResult: Item count: " <<  l.count() << endl;

  for (CDInfoList::ConstIterator it(l.begin()); it != l.end(); ++it)
  {
    CDInfo i(*it);

    kdDebug() << "Disc artist: `" << i.get("artist").toString() << "'" << endl;
    kdDebug() << "Disc title: `" << i.get("title").toString() << "'" << endl;
    kdDebug() << "Disc revision: `" << i.revision << "'" << endl;
  }

  if (!l.isEmpty())
  {
    kdDebug() << "---------------------------------------" << endl;
    kdDebug() << "Showing first item" << endl;

    CDInfo i(l.first());

    kdDebug() << "Disc artist: `" << i.get("artist").toString() << "'" << endl;
    kdDebug() << "Disc title: `" << i.get("title").toString() << "'" << endl;
    kdDebug() << "Disc genre: `" << i.get("genre").toString() << "'" << endl;
    kdDebug() << "Disc year: `" << i.get("year").toString() << "'" << endl;
    kdDebug() << "Disc length: `" << i.get("length").toString() << "'" << endl;
    kdDebug() << "Disc id: `" << i.get("discid").toString() << "'" << endl;
    kdDebug() << "Tracks........" << endl;

    for (TrackInfoList::ConstIterator it(i.trackInfoList.begin()); it != i.trackInfoList.end(); ++it)
    {
      kdDebug() << "  Track: `" << (*it).get(TrackInfo::Title).toString() << "'" << endl;
    }
    kdDebug() << "---------------------------------------" << endl;
  }

  CDInfo i(client_->bestLookupResponse());

  kdDebug() << "Best CDInfo had title: " << i.get("title").toString() << endl;
  kdDebug() << "and revision: " << i.revision << endl;

  kapp->quit();
}

int main(int argc, char ** argv)
{
  KCmdLineArgs::init(argc, argv, "libkcddb_test", "", "", "");

  KApplication app(false);

  AsyncCDDBLookupTest test;

  return app.exec();
}

#include "asynccddblookuptest.moc"

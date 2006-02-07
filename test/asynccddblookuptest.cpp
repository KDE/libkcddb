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
  kDebug() << "AsyncCDDBLookupTest::slotResult: Got " << KCDDB::CDDB::resultToString(r) << endl;

  CDInfoList l = client_->lookupResponse();

  kDebug() << "AsyncCDDBLookupTest::slotResult: Item count: " <<  l.count() << endl;

  for (CDInfoList::ConstIterator it(l.begin()); it != l.end(); ++it)
  {
    CDInfo i(*it);

    kDebug() << "Disc artist: `" << i.get("artist").toString() << "'" << endl;
    kDebug() << "Disc title: `" << i.get("title").toString() << "'" << endl;
    kDebug() << "Disc revision: `" << i.get("revision") << "'" << endl;
  }

  if (!l.isEmpty())
  {
    kDebug() << "---------------------------------------" << endl;
    kDebug() << "Showing first item" << endl;

    CDInfo info(l.first());

    kDebug() << "Disc artist: `" << info.get("artist").toString() << "'" << endl;
    kDebug() << "Disc title: `" << info.get("title").toString() << "'" << endl;
    kDebug() << "Disc genre: `" << info.get("genre").toString() << "'" << endl;
    kDebug() << "Disc year: `" << info.get("year").toString() << "'" << endl;
    kDebug() << "Disc length: `" << info.get("length").toString() << "'" << endl;
    kDebug() << "Disc id: `" << info.get("discid").toString() << "'" << endl;
    kDebug() << "Tracks........" << endl;

    for (int i=0; i < info.numberOfTracks(); i++)
    {
      kDebug() << "  Track: `" << info.track(i).get("title").toString() << "'" << endl;
    }
    kDebug() << "---------------------------------------" << endl;
  }

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

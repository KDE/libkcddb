#include <kdebug.h>
#include <kapplication.h>
#include <kcmdlineargs.h>

#include "asyncsmtpsubmittest.h"
#include "libkcddb/submit.h"

AsyncSMTPSubmitTest::AsyncSMTPSubmitTest()
  : QObject()
{
  using namespace KCDDB;

  client_ = new Client;
  client_->config().setSubmitTransport(Submit::SMTP);
  // Lookup sync
  client_->setBlockingMode( true );

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

  client_->lookup( list );

  client_->setBlockingMode( false );

  connect
    (
      client_,
      SIGNAL(finished(CDDB::Result)),
      SLOT(slotFinished(CDDB::Result))
    );

  client_->submit(client_->lookupResponse().first(), list);
}

  void
AsyncSMTPSubmitTest::slotFinished(CDDB::Result r)
{
  kdDebug() << "AsyncSMTPSubmitTest::slotFinished: Got " << KCDDB::CDDB::resultToString(r) << endl;

  kapp->quit();
}

int main(int argc, char ** argv)
{
  KCmdLineArgs::init(argc, argv, "libkcddb_test", "", "", "");

  KApplication app(false /* No styles */, false /* No GUI */);

  new AsyncSMTPSubmitTest;

  return app.exec();
}

#include "asyncsmtpsubmittest.moc"

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
  client_->config().setSubmitAddress("test-submit@freedb.org");
  client_->setBlockingMode( false );

  TrackOffsetList list;

  list
    << 150      // First track start.
    << 2592
    << 35472
    << 47891
    << 123310
    << 133125;  // Disc end.

  CDInfo cdInfo;

  cdInfo.set("discid", "3606ed05");
  cdInfo.set("revision", 4);
  cdInfo.set("title", "Bamse och Bronto");
  cdInfo.set("artist", "Musiksage");
  cdInfo.set("year", 2001);
  cdInfo.set("category", "misc");
  cdInfo.set("genre", "Barnsaga");
  cdInfo.set("extd", QString::fromUtf8("Berättare: Olof Thunberg"));

  cdInfo.track(0).set("title", "Bamses signaturmelodi");
  cdInfo.track(1).set("title","*");
  cdInfo.track(2).set("title", "Brummavisan");
  cdInfo.track(3).set("title", "*");
  cdInfo.track(4).set("title", QString::fromUtf8("Jätteödlan Bronto"));

  connect
    (
      client_,
      SIGNAL(finished(CDDB::Result)),
      SLOT(slotFinished(CDDB::Result))
    );

  client_->submit(cdInfo, list);
}

  void
AsyncSMTPSubmitTest::slotFinished(CDDB::Result r)
{
  kDebug() << "AsyncSMTPSubmitTest::slotFinished: Got " << KCDDB::CDDB::resultToString(r) << endl;

  kapp->quit();
}

int main(int argc, char ** argv)
{
  KCmdLineArgs::init(argc, argv, "libkcddb_test", "", "", "");

  KApplication app(false);

  new AsyncSMTPSubmitTest;

  return app.exec();
}

#include "asyncsmtpsubmittest.moc"

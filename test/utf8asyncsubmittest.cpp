#include <kdebug.h>
#include <kapplication.h>
#include <kcmdlineargs.h>

#include "utf8asyncsubmittest.h"
#include "libkcddb/submit.h"

Utf8AsyncSubmitTest::Utf8AsyncSubmitTest()
  : QObject()
{
  using namespace KCDDB;

  client_ = new Client;
  client_->config().setSubmitTransport(Submit::SMTP);
  client_->config().setSubmitAddress("test-submit@freedb.org");
  // Lookup sync
  client_->setBlockingMode( true );

  TrackOffsetList list;

  list
    << 150      // First track start.
    << 2592
    << 35472
    << 47891
    << 123310
    << 150       // Disc start.
    << 133125;  // Disc end.

  CDInfo cdInfo;

  cdInfo.id = "3606ed05";
  cdInfo.revision = 4;
  cdInfo.title  = "Bamse och Bronto";
  cdInfo.artist = "Musiksage";
  cdInfo.year   = 2001;
  cdInfo.category = "misc";
  cdInfo.genre  = "Barnsaga";
  cdInfo.extd = QString::fromUtf8("Berättare: Olof Thunberg");

  TrackInfo info;
  info.title = "Bamses signaturmelodi";
  cdInfo.trackInfoList.append(info);
  info.title = "*";
  cdInfo.trackInfoList.append(info);
  info.title = "Brummavisan";
  cdInfo.trackInfoList.append(info);
  info.title = "*";
  cdInfo.trackInfoList.append(info);
  info.title = QString::fromUtf8("Jätteödlan Bronto");
  cdInfo.trackInfoList.append(info);

  client_->setBlockingMode( false );

  connect
    (
      client_,
      SIGNAL(finished(CDDB::Result)),
      SLOT(slotFinished(CDDB::Result))
    );

  client_->submit(cdInfo, list);
}

  void
Utf8AsyncSubmitTest::slotFinished(CDDB::Result r)
{
  kdDebug() << k_funcinfo << ": Got " << KCDDB::CDDB::resultToString(r) << endl;

  kapp->quit();
}

int main(int argc, char ** argv)
{
  KCmdLineArgs::init(argc, argv, "libkcddb_test", "", "", "");

  KApplication app(false /* No styles */, false /* No GUI */);

  new Utf8AsyncSubmitTest;

  return app.exec();
}

#include "utf8asyncsubmittest.moc"

#include <qobject.h>
#include <libkcddb/client.h>

using namespace KCDDB;

class AsyncCDDBLookupTest : public QObject
{
  Q_OBJECT

  public:

    AsyncCDDBLookupTest();

  public slots:

    void slotFinished(Lookup::Result);

  private:

    KCDDB::Client * client_;
};

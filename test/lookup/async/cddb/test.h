#include <qobject.h>
#include <libkcddb/asyncclient.h>

using namespace KCDDB;

class AsyncCDDBLookupTest : public QObject
{
  Q_OBJECT

  public:

    AsyncCDDBLookupTest();

  protected slots:

    void slotLookupResponseReady(const QValueList<CDInfo> &);
    void slotError(Error);

  private:

    KCDDB::AsyncClient * client_;
};

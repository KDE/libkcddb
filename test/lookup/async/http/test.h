#include <qobject.h>
#include <libkcddb/asyncclient.h>

using namespace KCDDB;

class AsyncHTTPLookupTest : public QObject
{
  Q_OBJECT

  public:

    AsyncHTTPLookupTest();

  protected slots:

    void slotLookupResponseReady(const QValueList<CDInfo> &);
    void slotError(Error);

  private:

    KCDDB::AsyncClient * client_;
};

#include <qobject.h>
#include <libkcddb/asyncclient.h>

using namespace KCDDB;

class AsyncHTTPLookupTest : public QObject
{
  Q_OBJECT

  public:

    AsyncHTTPLookupTest();

  protected slots:

    void slotResult(KCDDB::Lookup::Result, const QValueList<CDInfo> &);

  private:

    KCDDB::AsyncClient * client_;
};

#ifndef TEST_H
#define TEST_H

#include <qobject.h>
#include <libkcddb/client.h>

using namespace KCDDB;

class AsyncHTTPLookupTest : public QObject
{
  Q_OBJECT

  public:

    AsyncHTTPLookupTest();
    ~AsyncHTTPLookupTest();

  public slots:

    void slotFinished(CDDB::Result);

  private:

    KCDDB::Client * client_;
};

#endif

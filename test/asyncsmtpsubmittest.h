#ifndef TEST_H
#define TEST_H

#include <qobject.h>
#include <libkcddb/client.h>

using namespace KCDDB;

class AsyncSMTPSubmitTest : public QObject
{
  Q_OBJECT

  public:
    AsyncSMTPSubmitTest();

  public slots:

    void slotFinished(CDDB::Result);

  private:

    KCDDB::Client * client_;
};

#endif

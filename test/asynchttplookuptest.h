#ifndef TEST_H
#define TEST_H

#include <QEventLoop>
#include <QObject>
#include <libkcddb/client.h>

using namespace KCDDB;

class AsyncHTTPLookupTest : public QObject
{
  Q_OBJECT
  private slots:
    void testLookup();
    void slotFinished(CDDB::Result);

  private:

    QEventLoop m_eventLoop;
    KCDDB::Client * client_;
    CDInfo m_info;
};

#endif

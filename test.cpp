#include <iostream>
#include <libkcddb/client.h>

int main()
{
  using namespace KCDDB;

  Client c;

  TrackOffsetList list;

  Error e = c.lookup(list);

  std::cerr << errorToString(e) << std::endl;

  return 0;
}

#include <iostream>

#include <kapplication.h>
#include <kcmdlineargs.h>

#include <libkcddb/client.h>

  int
main(int argc, char ** argv)
{
  KCmdLineArgs::init(argc, argv, "libkcddb_test", "", "");

  KApplication app(false /* No styles */, false /* No GUI */);

  using namespace KCDDB;

  Client c;

  TrackOffsetList list;

  // a1107d0a - Kruder & Dorfmeister - The K&D Sessions - Disc One.
  list
    << 150      // First track start.
    << 29462
    << 66983
    << 96785
    << 135628
    << 168676
    << 194147
    << 222158
    << 247076
    << 278203   // Last track start.
    << 10       // Disc start.
    << 316732;  // Disc end.

  std::cerr << "Stuff to send to server:" << std::endl;

  std::cerr
    << trackOffsetListToId(list)
    << " "
    << trackOffsetListToString(list)
    << std::endl;

  Error e = c.lookup(list);

  std::cerr << "Client::lookup returned error: " << errorToString(e)
    << std::endl;

  CDInfoList response = c.lookupResponse();

  std::cerr << "Client::lookup returned : " << response.count() << " entries"
    << std::endl;

  CDInfoList::ConstIterator it;

  for (it = response.begin(); it != response.end(); ++it)
  {
    CDInfo i(*it);

    std::cerr << "Disc title: " << i.title << std::endl;
  }

  // If we want to test submitions comment this line out
  return 0;

  it = response.begin();
  CDInfo i(*it);
  if(c.submit(i) == CannotSave){
    std::cerr << "Can not save\n";
  }

  return 0;
}

#ifndef SET_H
#define SET_H
#include <list>
#include <vector>
#include "addrinfo.h"
#include "line.h"

class Cache;

class Set
{
public:
  size_t cap;
  unsigned index;
  Cache *host;
  Set(size_t _cap, Cache *host, unsigned index);
  virtual ~Set() {}
  virtual void read(AddrInfo info, bool isData) = 0;
  virtual void write(AddrInfo info, bool is_write_back) = 0;
};

class LRUSet : public Set
{
public:
  std::list<Line> lines;
  LRUSet(size_t _cap, Cache *host, unsigned index);
  virtual ~LRUSet() {}
  virtual void read(AddrInfo info, bool isData);
  virtual void write(AddrInfo info, bool is_write_back);
};

/*
class RNDSet : public Set
{
public:
  std::vector<Line> lines;
  RNDSet(size_t _cap, Cache *host, unsigned index);
  virtual ~RNDSet();
  void read(AddrInfo info);
  void write(AddrInfo info);
};
*/

#endif
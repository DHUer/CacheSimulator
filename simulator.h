#ifndef SIMULATOR_H
#define SIMULATOR_H
#include "storage.h"
#include <fstream>
#include <iostream>

class CacheSimulator
{
public:
  Storage *l1_insn;
  Storage *l2_insn;
  Storage *l1_data;
  Storage *l2_data;
  Storage *dram;

  // the entry for each storage
  Storage *head_insn;
  Storage *head_data;

  int cache_mode1;        // 0/1/2 => none/split/unified
  int cache_mode2;        // 0/1/2 => none/split/unified
  int is_alloc;           // flag indicate whether write to cache when a write miss
  int len_addr;           // the length of addr
  int replace_algorithm1; // replace algorithm for l1, 0/1 => LRU/RND
  int replace_algorithm2; // replace algorithm for l2, 0/1 => LRU/RND

  std::ifstream ifs;

  CacheSimulator();
  ~CacheSimulator();
  void init();
  void run();
  void output();
  void createCache(Storage **insn_cache, Storage **data_cache, int replace_algorithm, int cache_mode, int len_addr);
  void linkCache(Storage **l1, Storage **l2);
};

#endif
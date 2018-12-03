#ifndef STORAGE_H
#define STORAGE_H
#include <vector>
#include <iostream>
#include "set.h"
#include <unordered_set>
#include <list>

class Storage
{
public:
  Storage *next;

  Storage();
  virtual ~Storage() {}

  // interfaces
  virtual void read(unsigned addr, bool isData) = 0;
  virtual void write(unsigned addr) = 0;
  virtual unsigned wrap(unsigned tag, unsigned index) = 0;
  virtual void output() = 0;

  // necessary functions
  unsigned len(unsigned);
  void set_next(Storage *);
};

class Cache : public Storage
{
public:
  unsigned a;                 // associality
  unsigned b;                 // block size
  unsigned c;                 // capacity
  unsigned is_alloc;          // is allocate on write miss?
  unsigned replace_algorithm; // 0/1 => LRU/RND

  unsigned len_addr;
  unsigned len_tag;
  unsigned len_index;
  unsigned len_offset;

  unsigned read_hit_data;
  unsigned read_miss_data;
  unsigned read_compulsory_miss_data;
  unsigned read_capacity_miss_data;
  unsigned read_conflict_miss_data;

  unsigned read_hit_insn;
  unsigned read_miss_insn;
  unsigned read_compulsory_miss_insn;
  unsigned read_capacity_miss_insn;
  unsigned read_conflict_miss_insn;

  unsigned write_hit_data;
  unsigned write_miss_data;
  unsigned write_compulsory_miss_data;
  unsigned write_capacity_miss_data;
  unsigned write_conflict_miss_data;

  std::unordered_set<unsigned> dict; // all occurred items
  std::list<unsigned> fully_associative_cache;

  std::vector<Set *> sets;

  Cache(unsigned a, unsigned b, unsigned c, unsigned is_alloc, unsigned replace_algorithm, unsigned len_addr);
  virtual ~Cache()
  {
    for (size_t i = 0; i < sets.size(); i++)
    {
      delete sets[i];
    }
  }

  virtual void read(unsigned addr, bool isData);
  virtual void write(unsigned addr);
  virtual unsigned wrap(unsigned tag, unsigned index);
  virtual void output();
  unsigned update_fully_associative_cache(unsigned key, bool alloc_flag);
};

class DRAM : public Storage
{
public:
  DRAM() {}
  virtual ~DRAM() {}
  virtual void read(unsigned addr, bool isData) {}
  virtual void write(unsigned addr) {}
  virtual unsigned wrap(unsigned tag, unsigned index) { return 0; }
  virtual void output() {}
};

#endif
#include "storage.h"

// ========================================================
Storage::Storage() : next(NULL), read_hit_data(0), read_miss_data(0), read_hit_insn(0), read_miss_insn(0), write_hit_data(0), write_miss_data(0) {}

unsigned Storage::len(unsigned num)
{
    return num == 1 ? 0 : (len(num / 2) + 1);
}

void Storage::set_next(Storage *next_storage)
{
    next = next_storage;
}

// ========================================================

Cache::Cache(unsigned a, unsigned b, unsigned c, unsigned is_alloc, unsigned replace_algorithm, unsigned len_addr) : a(a), b(b), c(c), is_alloc(is_alloc), replace_algorithm(replace_algorithm), len_addr(len_addr)
{
    if (a == 0)
        a = c / b;
    len_offset = len(b);
    len_index = len(c / b / a);
    len_tag = len_addr - len_offset - len_index;

    sets.resize(c / b / a, NULL);
    for (size_t i = 0; i < sets.size(); i++)
    {
        if (replace_algorithm == 0)
        { // LRU
            sets[i] = new LRUSet(a, this, i);
        }
        /*else if (replace_algorithm == 1)
        { // RND
            sets[i] = new RNDSet();
        }*/
    }
}

void Cache::read(unsigned addr, bool isData)
{
    AddrInfo info(addr, len_index, len_offset);
    sets[info.index]->read(info, isData);
}

void Cache::write(unsigned addr)
{
    AddrInfo info(addr, len_index, len_offset);
    sets[info.index]->write(info, is_alloc);
}

unsigned Cache::wrap(unsigned tag, unsigned index)
{
    unsigned res = 0;
    res = (tag << len_index) + index;
    res = (res << len_offset);
    return res;
}

// ========================================================

void DRAM::read(unsigned addr, bool isData) {}

void DRAM::write(unsigned addr) {}

unsigned DRAM::wrap(unsigned tag, unsigned index)
{
    return 0;
}

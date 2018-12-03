#include "storage.h"

// ========================================================
Storage::Storage() : next(NULL) {}

unsigned Storage::len(unsigned num)
{
    return num == 1 ? 0 : (len(num / 2) + 1);
}

void Storage::set_next(Storage *next_storage)
{
    next = next_storage;
}

// ========================================================

Cache::Cache(unsigned a, unsigned b, unsigned c, unsigned is_alloc, unsigned replace_algorithm, unsigned len_addr) : a(a), b(b), c(c), is_alloc(is_alloc), replace_algorithm(replace_algorithm), len_addr(len_addr), read_hit_data(0), read_miss_data(0), read_compulsory_miss_data(0), read_capacity_miss_data(0), read_conflict_miss_data(0), read_hit_insn(0), read_miss_insn(0), read_compulsory_miss_insn(0), read_capacity_miss_insn(0), read_conflict_miss_insn(0), write_hit_data(0), write_miss_data(0), write_compulsory_miss_data(0), write_capacity_miss_data(0), write_conflict_miss_data(0)
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

unsigned Cache::update_fully_associative_cache(unsigned key, bool alloc_flag)
{
    std::list<unsigned>::iterator it = fully_associative_cache.begin();
    while (it != fully_associative_cache.end())
    {
        if (*it == key)
        {
            unsigned key = *it;
            if (alloc_flag)
            {
                fully_associative_cache.erase(it);
                fully_associative_cache.push_front(key);
            }
            return 1; // hit in fully associative cache
        }
        it++;
    }
    if (it == fully_associative_cache.end())
    {
        if (alloc_flag)
        {
            if (fully_associative_cache.size() == c / b)
            {
                fully_associative_cache.pop_back();
            }
            fully_associative_cache.push_front(key);
        }
    }
    return 0; // not hit
}

void Cache::output()
{

    printf("Metrics                     Total           Instrn                  Data           Read           Write            Misc\n");
    printf("------                      -----           ------                  ----           ----           -----            ----\n");
    printf("Demand Fetches            %7u          %7u               %7u        %7u         %7u         %7u\n",
           read_hit_data + read_miss_data + write_hit_data + write_miss_data + read_hit_insn + read_miss_insn,
           read_hit_insn + read_miss_insn,
           read_hit_data + read_miss_data + write_hit_data + write_miss_data,
           read_hit_data + read_miss_data,
           write_hit_data + write_miss_data,
           0);
    printf(" Fraction of total         %.4f           %.4f                %.4f         %.4f          %.4f          %.4f\n",
           read_hit_data + read_miss_data + write_hit_data + write_miss_data + read_hit_insn + read_miss_insn == 0 ? 0.0 : (double)(read_hit_data + read_miss_data + write_hit_data + write_miss_data + read_hit_insn + read_miss_insn) / (read_hit_data + read_miss_data + write_hit_data + write_miss_data + read_hit_insn + read_miss_insn),
           read_hit_insn + read_miss_insn == 0 ? 0.0 : (double)(read_hit_insn + read_miss_insn) / (read_hit_data + read_miss_data + write_hit_data + write_miss_data + read_hit_insn + read_miss_insn),
           read_hit_data + read_miss_data + write_hit_data + write_miss_data == 0 ? 0.0 : (double)(read_hit_data + read_miss_data + write_hit_data + write_miss_data) / (read_hit_data + read_miss_data + write_hit_data + write_miss_data + read_hit_insn + read_miss_insn),
           read_hit_data + read_miss_data == 0 ? 0.0 : (double)(read_hit_data + read_miss_data) / (read_hit_data + read_miss_data + write_hit_data + write_miss_data + read_hit_insn + read_miss_insn),
           write_hit_data + write_miss_data == 0 ? 0.0 : (double)(write_hit_data + write_miss_data) / (read_hit_data + read_miss_data + write_hit_data + write_miss_data + read_hit_insn + read_miss_insn),
           (double)0);
    std::cout << std::endl;
    printf("Demand Misses             %7u          %7u               %7u        %7u         %7u         %7u\n",
           read_miss_data + write_miss_data + read_miss_insn,
           read_miss_insn,
           read_miss_data + write_miss_data,
           read_miss_data,
           write_miss_data,
           0);
    printf(" Demand Misses rate        %.4f           %.4f                %.4f         %.4f          %.4f          %.4f\n",
           read_hit_data + read_miss_data + write_hit_data + write_miss_data + read_hit_insn + read_miss_insn == 0 ? 0 : (double)(read_miss_data + write_miss_data + read_miss_insn) / (read_hit_data + read_miss_data + write_hit_data + write_miss_data + read_hit_insn + read_miss_insn),
           read_miss_insn == 0 ? 0.0 : (double)(read_miss_insn) / (read_hit_insn + read_miss_insn),
           read_miss_data + write_miss_data == 0 ? 0.0 : (double)(read_miss_data + write_miss_data) / (read_hit_data + read_miss_data + write_hit_data + write_miss_data),
           read_miss_data == 0 ? 0.0 : (double)(read_miss_data) / (read_hit_data + read_miss_data),
           write_miss_data == 0 ? 0.0 : (double)(write_miss_data) / (write_hit_data + write_miss_data),
           0.0);
    printf("  Compulsory misses       %7u          %7u               %7u        %7u         %7u         %7u\n",
           read_compulsory_miss_data + read_compulsory_miss_insn + write_compulsory_miss_data,
           read_compulsory_miss_insn,
           read_compulsory_miss_data + write_compulsory_miss_data,
           read_compulsory_miss_data,
           write_compulsory_miss_data,
           0);
    printf("  Capacity misses         %7u          %7u               %7u        %7u         %7u         %7u\n",
           read_capacity_miss_data + read_capacity_miss_insn + write_capacity_miss_data,
           read_capacity_miss_insn,
           read_capacity_miss_data + write_capacity_miss_data,
           read_capacity_miss_data,
           write_capacity_miss_data,
           0);
    printf("  Conflict misses         %7u          %7u               %7u        %7u         %7u         %7u\n",
           read_conflict_miss_data + read_conflict_miss_insn + write_conflict_miss_data,
           read_conflict_miss_insn,
           read_conflict_miss_data + write_conflict_miss_data,
           read_conflict_miss_data,
           write_conflict_miss_data,
           0);
}
// ========================================================

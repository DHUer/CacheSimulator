#include "storage.h"
#include <iostream>
#include <cstdio>

Set::Set(size_t _cap, Storage *host, unsigned index) : cap(_cap), host(host), index(index) {}

// ========================================================

LRUSet::LRUSet(size_t _cap, Storage *host, unsigned index) : Set(_cap, host, index)
{
    lines.resize(cap, Line());
}

void LRUSet::read(AddrInfo info, bool isData)
{
    std::list<Line>::iterator it = lines.begin();
    while (it != lines.end())
    {
        if (it->isValid && it->tag == info.tag)
        { // hit in current set
            Line line = *it;
            lines.erase(it);
            lines.push_front(line);

            if (isData)
                host->read_hit_data++;
            else
                host->read_hit_insn++;

            return;
        }
        ++it;
    }

    // miss in current set(cache)
    if (isData)
        host->read_miss_data++;
    else
        host->read_miss_insn++;

    host->next->read(info.addr, isData);

    Line line = lines.back();
    if (line.isDirty)
    {
        unsigned fake_addr = host->wrap(line.tag, index);
        host->next->write(fake_addr);
    }
    lines.pop_back();

    lines.push_front(Line(info.tag, true));
}

void LRUSet::write(AddrInfo info, bool is_alloc)
{

    std::list<Line>::iterator it = lines.begin();
    while (it != lines.end())
    {
        if (it->isValid && it->tag == info.tag)
        { // hit and do some modification
            Line line = *it;
            lines.erase(it);
            line.isDirty = true;
            lines.push_front(line);
            host->write_hit_data++;
            return;
        }
        ++it;
    }

    // miss in current set(cache)
    host->write_miss_data++;

    if (is_alloc)
    {
        host->next->read(info.addr, true);
        Line line = lines.back();
        if (line.isDirty)
        {
            unsigned fake_addr = host->wrap(line.tag, index);
            host->next->write(fake_addr);
        }
        lines.pop_back();
        lines.push_front(Line(info.tag, true, true));
    }
    else
    {
        host->next->write(info.addr);
    }
}

void Cache::output()
{
    
    printf("Metrics                      Total           Instrn                 Data            Read           Write            Misc\n");
    printf("------                       -----           ------                 ----            ----           -----            ----\n");
    printf("Demand Fetches            %7u            %5u                 %5u          %5u           %5u           %5u\n",
           read_hit_data + read_miss_data + write_hit_data + write_miss_data + read_hit_insn + read_miss_insn,
           read_hit_insn + read_miss_insn,
           read_hit_data + read_miss_data + write_hit_data + write_miss_data,
           read_hit_data + read_miss_data,
           write_hit_data + write_miss_data,
           0);
    printf("Fraction of total          %.4f           %.4f                %.4f         %.4f          %.4f          %.4f\n",
           (double)(read_hit_data + read_miss_data + write_hit_data + write_miss_data + read_hit_insn + read_miss_insn) / (read_hit_data + read_miss_data + write_hit_data + write_miss_data + read_hit_insn + read_miss_insn),
           (double)(read_hit_insn + read_miss_insn) / (read_hit_data + read_miss_data + write_hit_data + write_miss_data + read_hit_insn + read_miss_insn),
           (double)(read_hit_data + read_miss_data + write_hit_data + write_miss_data) / (read_hit_data + read_miss_data + write_hit_data + write_miss_data + read_hit_insn + read_miss_insn),
           (double)(read_hit_data + read_miss_data) / (read_hit_data + read_miss_data + write_hit_data + write_miss_data + read_hit_insn + read_miss_insn),
           (double)(write_hit_data + write_miss_data) / (read_hit_data + read_miss_data + write_hit_data + write_miss_data + read_hit_insn + read_miss_insn),
           (double)0);
    std::cout << std::endl;
    printf("Demand Misses             %7u            %5u                 %5u          %5u           %5u           %5u\n",
           read_miss_data + write_miss_data + read_miss_insn,
           read_miss_insn,
           read_miss_data + write_miss_data,
           read_miss_data,
           write_miss_data,
           0);
    printf("Demand Misses rate         %.4f           %.4f                %.4f         %.4f          %.4f          %.4f\n",
           (double)(read_miss_data + write_miss_data + read_miss_insn) / (read_hit_data + read_miss_data + write_hit_data + write_miss_data + read_hit_insn + read_miss_insn),
           (double)(read_miss_insn) / (read_hit_insn + read_miss_insn),
           (double)(read_miss_data + write_miss_data) / (read_hit_data + read_miss_data + write_hit_data + write_miss_data),
           (double)(read_miss_data) / (read_hit_data + read_miss_data),
           (double)(write_miss_data) / (write_hit_data + write_miss_data),
           0.0);
}
// ========================================================
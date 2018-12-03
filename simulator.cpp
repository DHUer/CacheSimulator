#include "simulator.h"

CacheSimulator::CacheSimulator() : l1_insn(NULL), l2_insn(NULL), l1_data(NULL), l2_data(NULL), dram(NULL)
{
    // Configuration
    len_addr = 24;
    cache_mode1 = 1; // 0/1/2 => none/split/unified
    cache_mode2 = 1; // 0/1/2 => none/split/unified
    is_alloc = 1;
    replace_algorithm1 = 0; // 0/1 => LRU/RND
    replace_algorithm2 = 0; // 0/1 => LRU/RND
    /*
    1 64 8192
    4 64 16384
    8 64 32768
    16 64 65536
    */
}

void CacheSimulator::init()
{
    createCache(&l1_insn, &l1_data, replace_algorithm1, cache_mode1, len_addr); // level 1
    createCache(&l2_insn, &l2_data, replace_algorithm2, cache_mode2, len_addr); // level 2

    dram = new DRAM();

    // link them together
    linkCache(&l1_insn, &l2_insn);
    linkCache(&l1_data, &l2_data);
    head_data = l1_data ? l1_data : (l2_data ? l2_data : dram);
    head_insn = l1_insn ? l1_insn : (l2_insn ? l2_insn : dram);

    // Open file
    std::string filename;
    std::cout << "Filename: ";
    //std::cin >> filename;
    filename = "DineroFull.din";
    std::cout << filename << "\n";
    ifs.open(filename.c_str(), std::ifstream::in);
}

void CacheSimulator::run()
{
    init();

    int op;
    unsigned addr;
    while (ifs.good())
    {
        ifs >> op;
        ifs >> std::hex >> addr;
        if (op == 0)
        { // data read
            linkCache(&l1_data, &l2_data);
            head_data->read(addr, op < 2);
        }
        else if (op == 1)
        { // data write
            linkCache(&l1_data, &l2_data);
            head_data->write(addr);
        }
        else if (op == 2)
        { // insn fetch
            linkCache(&l1_insn, &l2_insn);
            head_insn->read(addr, op < 2);
        }
    }

    output();
}

CacheSimulator::~CacheSimulator()
{
    ifs.close();
    if (cache_mode1 == 1)
    {
        delete l1_insn;
        delete l1_data;
    }
    else if (cache_mode1 == 2)
    {
        delete l1_data;
    }

    if (cache_mode2 == 1)
    {
        delete l2_insn;
        delete l2_data;
    }
    else if (cache_mode2 == 2)
    {
        delete l2_data;
    }

    delete dram;
}

void CacheSimulator::createCache(Storage **insn_cache, Storage **data_cache, int replace_algorithm, int cache_mode, int len_addr)
{
    int a, b, c;

    if (cache_mode == 1)
    { // split
        std::cout << "Input ABC(a b c) for instruction cache: ";
        std::cin >> a >> b >> c;
        *insn_cache = new Cache(a, b, c, is_alloc, replace_algorithm, len_addr);
        std::cout << "Input ABC(a b c) for data cache: ";
        std::cin >> a >> b >> c;
        *data_cache = new Cache(a, b, c, is_alloc, replace_algorithm, len_addr);
    }
    else if (cache_mode == 2)
    { // unified
        std::cout << "Input ABC(a b c) for cache: ";
        std::cin >> a >> b >> c;
        // a = 1;
        // b = 32;
        // c = 8192;
        *data_cache = *insn_cache = new Cache(a, b, c, is_alloc, replace_algorithm, len_addr);
    }
}

void CacheSimulator::linkCache(Storage **l1, Storage **l2)
{
    if ((*l1) && (*l2))
    {
        (*l1)->set_next((*l2));
        (*l2)->set_next(dram);
    }
    else if ((*l1) && !(*l2))
    {
        (*l1)->set_next(dram);
    }
    else if (!(*l1) && (*l2))
    {
        (*l2)->set_next(dram);
    }
}

void CacheSimulator::output()
{
    std::cout << "=======================================================================================================================\n";
    std::cout << "Level 1 mode: ";
    if (cache_mode1 == 1)
    {
        std::cout << "split, " << (is_alloc ? "" : "not") << "write alloc"
                  << "\n";
        std::cout << "Level 1 Instruction Cache: \n";
        l1_insn->output();
        std::cout << "\nLevel 1 Data Cache: \n";
        l1_data->output();
    }
    else if (cache_mode1 == 2)
    {
        std::cout << "united, " << (is_alloc ? "" : "not") << "write alloc"
                  << "\n";
        std::cout << "Cache: \n";
        l1_insn->output();
    }
    else
    {
        std::cout << "none\n";
    }
    std::cout << "=======================================================================================================================\n";
    std::cout << "Level 2 mode: ";
    if (cache_mode2 == 1)
    {
        std::cout << "split, " << (is_alloc ? "" : "not") << "write alloc"
                  << "\n";
        std::cout << "Level 2 Instruction Cache: \n";
        l2_insn->output();
        std::cout << "\nLevel 2 Data Cache: \n";
        l2_data->output();
    }
    else if (cache_mode2 == 2)
    {
        std::cout << "united, " << (is_alloc ? "" : "not") << "write alloc"
                  << "\n";
        std::cout << "Cache: \n";
        l2_insn->output();
    }
    else
    {
        std::cout << "none\n";
    }
    std::cout << "=======================================================================================================================\n";
}
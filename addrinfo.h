#ifndef ADDRINFO_H
#define ADDRINFO_H
class AddrInfo
{
  public:
    int tag;
    int index;
    int offset;
    unsigned addr;

    AddrInfo(unsigned _addr, int len_index, int len_offset) : addr(_addr)
    {
        offset = _addr & ((1 << len_offset) - 1);
        _addr = _addr >> len_offset;
        index = _addr & ((1 << len_index) - 1);
        _addr = _addr >> len_index;
        tag = _addr;
    }
};
#endif
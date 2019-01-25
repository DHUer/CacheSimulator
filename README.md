# CacheSimulator

## Background

This project is for Duke ECE550 18fall, the requirements available at [Lab4.pdf](https://github.com/menyf/CacheSimulator/blob/master/Lab%204.pdf) and [Testing Specification](https://github.com/menyf/CacheSimulator/blob/master/Testing%20Specification.pdf)

This project is developed by the following people

- Yifan Men yifan.men@duke.edu
- Yukun Yang yukun.yang@duke.edu
- Hongliang Dong hongliang.dong@duke.edu

It simulates Cache functions and also calculate the rate of hit and miss.

This version only implements LRU policy. RND policy and other policies should be developed further.

## Usage

Several parameter should be assigned in simulator.cpp.

The result of simulation:

```
~/Developer/CacheSimulator $ ./myCacheSimulator
Input ABC(a b c) for instruction cache: 1 64 8192
Input ABC(a b c) for data cache: 4 64 16384
Input ABC(a b c) for instruction cache: 8 64 32768
Input ABC(a b c) for data cache: 16 64 65536
Filename: DineroFull.din
=======================================================================================================================
Level 1 mode: split, write alloc
Level 1 Instruction Cache:
Metrics                     Total           Instrn                  Data           Read           Write            Misc
------                      -----           ------                  ----           ----           -----            ----
Demand Fetches            5689552          5689552                     0              0               0               0
 Fraction of total         1.0000           1.0000                0.0000         0.0000          0.0000          0.0000

Demand Misses                 155              155                     0              0               0               0
 Demand Misses rate        0.0000           0.0000                0.0000         0.0000          0.0000          0.0000
  Compulsory misses           144              144                     0              0               0               0
  Capacity misses               1                1                     0              0               0               0
  Conflict misses              10               10                     0              0               0               0

Level 1 Data Cache:
Metrics                     Total           Instrn                  Data           Read           Write            Misc
------                      -----           ------                  ----           ----           -----            ----
Demand Fetches            1769119                0               1769119        1145629          623490               0
 Fraction of total         1.0000           0.0000                1.0000         0.6476          0.3524          0.0000

Demand Misses               38082                0                 38082          34968            3114               0
 Demand Misses rate        0.0215           0.0000                0.0215         0.0305          0.0050          0.0000
  Compulsory misses          1797                0                  1797            142            1655               0
  Capacity misses           29261                0                 29261          27815            1446               0
  Conflict misses            7024                0                  7024           7011              13               0
=======================================================================================================================
Level 2 mode: split, write alloc
Level 2 Instruction Cache:
Metrics                     Total           Instrn                  Data           Read           Write            Misc
------                      -----           ------                  ----           ----           -----            ----
Demand Fetches                155              155                     0              0               0               0
 Fraction of total         1.0000           1.0000                0.0000         0.0000          0.0000          0.0000

Demand Misses                 144              144                     0              0               0               0
 Demand Misses rate        0.9290           0.9290                0.0000         0.0000          0.0000          0.0000
  Compulsory misses           144              144                     0              0               0               0
  Capacity misses               0                0                     0              0               0               0
  Conflict misses               0                0                     0              0               0               0

Level 2 Data Cache:
Metrics                     Total           Instrn                  Data           Read           Write            Misc
------                      -----           ------                  ----           ----           -----            ----
Demand Fetches              41181                0                 41181          38082            3099               0
 Fraction of total         1.0000           0.0000                1.0000         0.9247          0.0753          0.0000

Demand Misses                3857                0                  3857           3849               8               0
 Demand Misses rate        0.0937           0.0000                0.0937         0.1011          0.0026          0.0000
  Compulsory misses          1797                0                  1797           1797               0               0
  Capacity misses            1712                0                  1712           1706               6               0
  Conflict misses             348                0                   348            346               2               0
=======================================================================================================================

```
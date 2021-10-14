#include <emp-tool/emp-tool.h>
#include "emp-ot/emp-ot.h"
#include <iostream>
using namespace emp;

template <typename T>
double ot_demo(T *ot, NetIO *io, int party, int64_t length)
{
    // 创建三个长度为 length 的 block 数组，b0 和 b1 归 sender 所有，r 归 reciver 所有
    block *b0 = new block[length], *b1 = new block[length], *r = new block[length];
    PRG prg(fix_key);

    *b0 = _mm_set1_epi32(0);
    *b1 = _mm_set1_epi32(1);
    
    // 创建一个长度为 length 的 Boolean 数组，每一位都是一个 choice bit
    // 当第 i 位 choice bit 是 0 的时候选择 b0[i]，第 i 位是 1 的时候选择 b1[i]
    bool *c = new bool[length];

    PRG prg2;
    prg2.random_bool(c, length);

    auto start = clock_start();
    if (party == ALICE)
    {
        ot->send(b0, b1, length);
    }
    else
    {
        ot->recv(r, c, length);
    }
    io->flush();
    long long t = time_from(start);
    // if (party == BOB)
    // {
    //     for (int64_t i = 0; i < length; ++i)
    // }

    std::cout << "Tests passed.\t";
	delete[] b0;
	delete[] b1;
	delete[] r;
	delete[] b;
	return t;
}
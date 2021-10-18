// #include<emmintrin.h>

#include <emp-tool/emp-tool.h>
#include "emp-ot/emp-ot.h"

#include <cstdint>
#include <string>
#include <cstdio>
#include <iostream>

using namespace emp;


void print128_num(__m128i var)
{
    uint32_t val[4];
    memcpy(val, &var, sizeof(val));

    char buffer[50];
    sprintf(buffer, "Numerical: %i", val[0]);
    std::cout << buffer << std::endl;
}



template <typename T>
double my_test_ot(T *ot, NetIO *io, int party, int64_t length)
{
    // 创建三个长度为 length 的 block 数组，b0 和 b1 属于 sender，r 属于 receiver
    block *b0 = new block[length], *b1 = new block[length], *r = new block[length];
    PRG prg(fix_key);

    // 对 b0 和 b1 进行初始化
    for (int64_t i = 0; i < length; ++i){
        *(b0 + i) = _mm_set1_epi32(0);
        *(b1 + i) = _mm_set1_epi32(1);
    }

    // 创建一个长度为 length 的 Boolean 数组，每一位都是一个 choice bit
    // 当第 i 位 choice bit 是 0 的时候选择 b0[i]，第 i 位是 1 的时候选择 b1[i]
    bool *c = new bool[length];
    // 对 c 进行初始化，为方便检查，将其初始化为交替的 false 和 true。
    for (int64_t i = 0; i < length; ++i){
        *(c+i) = bool(i%2);
    }

    auto start = clock_start();
    // 如果执行的进程的 party 是 ALICE，则发送 b0, b1，若不是则进行接收
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

    // 当进程的 party 是 BOB 的时候，打印一下接收到的数组，看看是否是自己想要的
    if (party == BOB)
    {
        for (int64_t i = 0; i < length; ++i){
            print128_num(*(r+i));
        }
    }

    std::cout << "Tests passed.\t";
	delete[] b0;
	delete[] b1;
	delete[] r;
	delete[] c;
	return t;
}
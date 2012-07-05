#include <iostream>
#include <algorithm>

#include "Generator.h"

namespace
{
    GENERATOR(Generator, double, int, x)
    {
        const int count = x;
        for(int i = 0; i < count; ++i)
        {
            YIELD(i*i);
        }
    }
}

int main()
{
    for(Generator::Iterator iter = Generator::begin(10),
        endIter = Generator::end(); iter != endIter; ++iter)
    {
        std::cout<<*iter<<std::endl;
    }

    Generator::Iterator iter = Generator::begin(11);
    Generator::Iterator endIter = Generator::end();
    while(iter != endIter)
    {
        std::cout<<*iter++<<std::endl;
    }

    std::copy(Generator::begin(12), Generator::end(),
        std::ostream_iterator<double>(std::cout, " "));
    std::cout<<std::endl;

    {
        Generator::Iterator i1 = Generator::begin(10);
        Generator::Iterator i2 = i1;
        std::cout<<(i1 == i2?"equal":"not equal")<<std::endl;
        std::cout<<(i1++ == i2?"equal":"not equal")<<std::endl;
        std::cout<<(i1 == i2?"equal":"not equal")<<std::endl;
    }

    return 0;
}

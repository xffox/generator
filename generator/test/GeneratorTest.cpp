#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include "Generator.h"

namespace generator
{
    namespace test
    {

    GENERATOR(EmptyGenerator, int, int, arg)
    {
        (void)arg;
    }

    GENERATOR(SquaresGenerator, long, int, count)
    {
        for(int i = 1; i <= count; ++i)
            YIELD(i*i);
    }

    class Incrementor
    {
    public:
        Incrementor()
            :value(1)
        {
        }

        void increment()
        {
            ++value;
        }

        int getValue() const
        {
            return value;
        }

    private:
        int value;
    };

    GENERATOR(IncrementorGenerator, Incrementor, int, count)
    {
        Incrementor incrementor;
        for(int i = 0; i < count; ++i)
        {
            YIELD(incrementor);
            incrementor.increment();
        }
    }

    class GeneratorTest: public CppUnit::TestCase
    {
        CPPUNIT_TEST_SUITE(GeneratorTest);
        CPPUNIT_TEST(testEmptyGenerator);
        CPPUNIT_TEST(testEndEquality);
        CPPUNIT_TEST(testDereference);
        CPPUNIT_TEST(testArrow);
        CPPUNIT_TEST(testCopyConstruction);
        CPPUNIT_TEST(testAssignment);
        CPPUNIT_TEST(testPrefixIncrement);
        CPPUNIT_TEST(testPostfixIncrement);
        CPPUNIT_TEST_SUITE_END();

    public:
        void testEmptyGenerator()
        {
            EmptyGenerator::Iterator iter = EmptyGenerator::begin(0);
            CPPUNIT_ASSERT(iter == EmptyGenerator::end());
        }

        void testEndEquality()
        {
            SquaresGenerator::Iterator endIter = SquaresGenerator::end();
            CPPUNIT_ASSERT(endIter == SquaresGenerator::end());
        }

        void testDereference()
        {
            SquaresGenerator::Iterator iter = SquaresGenerator::begin(1);
            CPPUNIT_ASSERT(*iter == 1);
        }

        void testArrow()
        {
            IncrementorGenerator::Iterator iter =
                IncrementorGenerator::begin(1);
            CPPUNIT_ASSERT(iter->getValue() == 1);
        }

        void testCopyConstruction()
        {
            SquaresGenerator::Iterator aIter = SquaresGenerator::begin(1);
            CPPUNIT_ASSERT(aIter != SquaresGenerator::end());
            SquaresGenerator::Iterator bIter(aIter);
            CPPUNIT_ASSERT(aIter == bIter);
            CPPUNIT_ASSERT(*aIter == *bIter);
        }

        void testAssignment()
        {
            SquaresGenerator::Iterator aIter = SquaresGenerator::begin(1);
            SquaresGenerator::Iterator bIter;
            CPPUNIT_ASSERT(aIter != bIter);
            bIter = aIter;
            CPPUNIT_ASSERT(aIter == bIter);
            CPPUNIT_ASSERT(*aIter == *bIter);
        }

        void testPrefixIncrement()
        {
            const int count = 10;
            SquaresGenerator::Iterator iter = SquaresGenerator::begin(count);
            SquaresGenerator::Iterator endIter = SquaresGenerator::end();
            for(int i = 1; i <= count; ++i)
            {
                CPPUNIT_ASSERT(iter != endIter);
                CPPUNIT_ASSERT(*iter == i*i);
                ++iter;
            }
            CPPUNIT_ASSERT(iter == endIter);
        }

        void testPostfixIncrement()
        {
            SquaresGenerator::Iterator aIter = SquaresGenerator::begin(2);
            SquaresGenerator::Iterator iter(aIter);
            SquaresGenerator::Iterator bIter(iter++);
            CPPUNIT_ASSERT(aIter == bIter);
            aIter = iter;
            bIter = iter++;
            CPPUNIT_ASSERT(aIter == bIter);
            CPPUNIT_ASSERT(iter == SquaresGenerator::end());
        }
    };

    CPPUNIT_TEST_SUITE_REGISTRATION(GeneratorTest);
    }
}

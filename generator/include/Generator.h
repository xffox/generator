#ifndef GENERATOR_GENERATOR_H
#define GENERATOR_GENERATOR_H

#include "GeneratorIterator.h"

#define GENERATOR(name, OutType, InType, inName) \
    class name \
    { \
    public: \
        typedef generator::GeneratorIterator<OutType, InType> Iterator; \
    public: \
        static Iterator begin(const InType &inValue) \
        { \
            return Iterator(func, inValue); \
        } \
        static Iterator end() \
        { \
            return Iterator(); \
        } \
    private: \
        static void func(Iterator::Context*, const InType&); \
    }; \
    void name::func(Iterator::Context *___generatorContext, \
        const InType &inName)

#define YIELD(val) \
    if(!___generatorContext->setValue(val)) \
        return;

#endif

#ifndef GENERATOR_GENERATORITERATOR_H
#define GENERATOR_GENERATORITERATOR_H

#include <iterator>
#include <memory>
#include <cassert>

#include "GeneratorContext.h"

namespace generator
{
    template<typename OutType, typename InType>
    class GeneratorIterator:
        public std::iterator<std::input_iterator_tag, OutType>
    {
        template<typename R, typename S>
        friend bool operator==(const GeneratorIterator<R, S> &left,
            const GeneratorIterator<R, S> &right);

    public:
        typedef GeneratorContext<OutType, InType> Context;
        typedef typename GeneratorContext<OutType, InType>::GeneratorFunc Func;

    public:
        GeneratorIterator();
        GeneratorIterator(Func func, const InType &inValue);

        GeneratorIterator &operator++()
        {
            if(context)
            {
                context->continueGenerator();
                onStep();
            }
            return *this;
        }

        GeneratorIterator operator++(int)
        {
            GeneratorIterator prev(*this);
            operator++();
            return prev;
        }

        const OutType &operator*() const
        {
            return value;
        }

        const OutType *operator->() const
        {
            return &value;
        }

    private:
        void onStep();
        void clearFinishedContext();

    private:
        std::shared_ptr<Context> context;
        OutType value;
    };

    template<typename OutType, typename InType>
    GeneratorIterator<OutType, InType>::GeneratorIterator()
        :context(), value()
    {
    }

    template<typename OutType, typename InType>
    GeneratorIterator<OutType, InType>::GeneratorIterator(Func func,
        const InType &inValue)
        :context(new Context(func, inValue)), value()
    {
        onStep();
    }

    template<typename OutType, typename InType>
    void GeneratorIterator<OutType, InType>::onStep()
    {
        assert(context);
        value = context->getValue();
        clearFinishedContext();
    }

    template<typename OutType, typename InType>
    void GeneratorIterator<OutType, InType>::clearFinishedContext()
    {
        assert(context);
        if(context->isFinished())
            context.reset();
    }

    template<typename OutType, typename InType>
    bool operator==(const GeneratorIterator<OutType, InType> &left,
        const GeneratorIterator<OutType, InType> &right)
    {
        return left.context == right.context &&
            (!left.context || (left.value == right.value));
    }

    template<typename OutType, typename InType>
    bool operator!=(const GeneratorIterator<OutType, InType> &left,
        const GeneratorIterator<OutType, InType> &right)
    {
        return !(left == right);
    }
}

#endif

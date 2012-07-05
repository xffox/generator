#ifndef GENERATOR_GENERATORCONTEXT_H
#define GENERATOR_GENERATORCONTEXT_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <cassert>

namespace generator
{
    template<typename OutType, typename InType>
    class GeneratorContext
    {
    public:
        typedef void (*GeneratorFunc)(GeneratorContext*, const InType&);

    public:
        GeneratorContext(GeneratorFunc func, const InType &inValue);
        ~GeneratorContext();

        bool isFinished() const;

        void breakGenerator();
        void continueGenerator();

        const OutType &getValue() const;
        bool setValue(const OutType &value);

    private:
        void run();

        void switchToGenerator(std::unique_lock<std::mutex> &lock);
        void switchToIterator(std::unique_lock<std::mutex> &lock);

        void switchExecution(std::condition_variable &contCondition,
            std::condition_variable &waitCondition,
            std::unique_lock<std::mutex> &lock);

        void generatorFinished();
        void contIterator();
        void contGenerator();

        static void routine(GeneratorContext *context);

    private:
        const GeneratorFunc func;
        const InType inValue;
        bool cont;
        OutType value;

        std::mutex mutex;
        std::condition_variable generatorCondition;
        std::condition_variable iteratorCondition;
        std::thread thread;
    };

    template<typename OutType, typename InType>
    GeneratorContext<OutType, InType>::GeneratorContext(GeneratorFunc func,
        const InType &inValue)
        :func(func), inValue(inValue), cont(true), value(),
        mutex(), generatorCondition(), iteratorCondition(), thread()
    {
        std::unique_lock<std::mutex> lock(mutex);

        thread = std::thread(routine, this);

        switchToGenerator(lock);
    }

    template<typename OutType, typename InType>
    GeneratorContext<OutType, InType>::~GeneratorContext()
    {
        breakGenerator();

        if(thread.joinable())
            thread.join();
    }

    template<typename OutType, typename InType>
    void GeneratorContext<OutType, InType>::run()
    {
        func(this, inValue);
        generatorFinished();
    }

    template<typename OutType, typename InType>
    bool GeneratorContext<OutType, InType>::isFinished() const
    {
        return !cont;
    }

    template<typename OutType, typename InType>
    void GeneratorContext<OutType, InType>::switchToGenerator(
        std::unique_lock<std::mutex> &lock)
    {
        switchExecution(generatorCondition, iteratorCondition, lock);
    }

    template<typename OutType, typename InType>
    void GeneratorContext<OutType, InType>::switchToIterator(
        std::unique_lock<std::mutex> &lock)
    {
        switchExecution(iteratorCondition, generatorCondition, lock);
    }

    template<typename OutType, typename InType>
    void GeneratorContext<OutType, InType>::breakGenerator()
    {
        std::unique_lock<std::mutex> lock(mutex);

        cont = false;

        contGenerator();
    }

    template<typename OutType, typename InType>
    void GeneratorContext<OutType, InType>::generatorFinished()
    {
        std::unique_lock<std::mutex> lock(mutex);

        cont = false;

        contIterator();
    }

    template<typename OutType, typename InType>
    void GeneratorContext<OutType, InType>::continueGenerator()
    {
        std::unique_lock<std::mutex> lock(mutex);

        switchToGenerator(lock);
    }

    template<typename OutType, typename InType>
    const OutType &GeneratorContext<OutType, InType>::getValue() const
    {
        return value;
    }

    template<typename OutType, typename InType>
    bool GeneratorContext<OutType, InType>::setValue(const OutType &value)
    {
        std::unique_lock<std::mutex> lock(mutex);

        this->value = value;

        switchToIterator(lock);

        return cont;
    }

    template<typename OutType, typename InType>
    void GeneratorContext<OutType, InType>::switchExecution(
        std::condition_variable &contCondition,
        std::condition_variable &waitCondition,
        std::unique_lock<std::mutex> &lock)
    {
        contCondition.notify_one();
        waitCondition.wait(lock);
    }

    template<typename OutType, typename InType>
    void GeneratorContext<OutType, InType>::contIterator()
    {
        iteratorCondition.notify_one();
    }

    template<typename OutType, typename InType>
    void GeneratorContext<OutType, InType>::contGenerator()
    {
        generatorCondition.notify_one();
    }

    template<typename OutType, typename InType>
    void GeneratorContext<OutType, InType>::routine(GeneratorContext *context)
    {
        assert(context);
        context->run();
    }
}

#endif

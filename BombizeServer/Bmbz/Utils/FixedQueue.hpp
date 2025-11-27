#ifndef BMBZ_UTILS_FIXEDQUEUE_HPP
#define BMBZ_UTILS_FIXEDQUEUE_HPP

#include <array>

namespace Bmbz
{
    namespace Utils
    {
        template<typename T, size_t sizeQueue>
        class FixedQueue
        {
        public:
            FixedQueue() = default;



        private:
            std::array<T, sizeQueue> _queue;
            //size_t pointer
        };
    }
}

#endif
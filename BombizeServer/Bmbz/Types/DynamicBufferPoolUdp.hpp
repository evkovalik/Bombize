#ifndef BMBZ_TYPE_DYNAMICBUFFERPOOLUDP_HPP
#define BMBZ_TYPE_DYNAMICBUFFERPOOLUDP_HPP

#include "SharedBufferWithHeader.hpp"

namespace Bmbz
{
    namespace Type
    {
        class DynamicBufferPoolUdp
        {
        public:
            DynamicBufferPoolUdp() = default;

            DynamicBufferPoolUdp(size_t quantityLim, size_t maxSizeBufferInPool)
            : _quantityLim(quantityLim)
            , _sizeBufferInPool(maxSizeBufferInPool)
            {}

            std::shared_ptr<SharedBufferWithHeader> GetFreeBuffer()
            {
                for(auto buf : _buffers)
                {
                    if(!buf->busy)
                    {
                        buf->Clear();
                        return buf;
                    }
                }

                if(_buffers.size() < _quantityLim || _quantityLim == 0)
                    return (_sizeBufferInPool != 0) ? 
                        _buffers.emplace_back(std::make_shared<SharedBufferWithHeader>(_sizeBufferInPool)) 
                        : _buffers.emplace_back(std::make_shared<SharedBufferWithHeader>());
                else
                    return nullptr;
            }

            std::shared_ptr<SharedBufferWithHeader> GetAndOccupyFreeBuffer()
            {
                auto buf = GetFreeBuffer();
                if(buf) buf->busy = true;
                return buf;
            }

            size_t Size() const
            {
                return _buffers.size();
            }

            int CountFree() const
            {
                int count = 0;
                for(auto buf : _buffers)
                {
                    if(!buf->busy) count++;
                }
                return count;
            }

        private:
            std::vector<std::shared_ptr<SharedBufferWithHeader>> _buffers;
            size_t _quantityLim = 0;
            size_t _sizeBufferInPool = 0;
        };
    }
}

#endif
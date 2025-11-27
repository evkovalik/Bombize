#ifndef BMBZ_TYPE_DYNAMICBUFFERPOOL_HPP
#define BMBZ_TYPE_DYNAMICBUFFERPOOL_HPP

#include "SharedBuffer.hpp"

namespace Bmbz
{
    namespace Type
    {
        class DynamicBufferPool
        {
        public:
            DynamicBufferPool() = default;

            DynamicBufferPool(size_t quantityLim, size_t maxSizeBufferInPool)
            : _quantityLim(quantityLim)
            , _sizeBufferInPool(maxSizeBufferInPool)
            {}

            std::shared_ptr<SharedBuffer> GetFreeBuffer()
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
                        _buffers.emplace_back(std::make_shared<SharedBuffer>(_sizeBufferInPool)) 
                        : _buffers.emplace_back(std::make_shared<SharedBuffer>());
                else
                    return nullptr;
            }

            std::shared_ptr<SharedBuffer> GetAndOccupyFreeBuffer()
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
            std::vector<std::shared_ptr<SharedBuffer>> _buffers;
            size_t _quantityLim = 0;
            size_t _sizeBufferInPool = 0;
        };
    }
}

#endif
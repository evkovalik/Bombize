#ifndef BMBZ_TYPE_SHAREDBUFFER_HPP
#define BMBZ_TYPE_SHAREDBUFFER_HPP

#include <boost/beast/core/flat_buffer.hpp>

namespace Bmbz
{
    namespace Type
    {
        class SharedBuffer
        {
        public:
            SharedBuffer() = default;
            SharedBuffer(std::size_t limitBody) : body(limitBody) {}

            boost::beast::flat_buffer body;
            bool autoFree = true;
            bool busy = false;

            const boost::beast::flat_buffer& GetSendData() const
            {
                return body;
            }

            bool Sent() const
            {
                return _sent;
            }

            void Sent(bool setValue)
            {
                _sent = setValue;
                if(autoFree)
                    busy = false;
            }

            void Reset()
            {
                autoFree = true;
                busy = false;
                _sent = false;
            }

            void Clear()
            {
                body.consume(body.size());
            }

        private:
            bool _sent = false;
        };
    }
}

#endif
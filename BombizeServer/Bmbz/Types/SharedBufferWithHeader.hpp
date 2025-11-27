#ifndef BMBZ_TYPE_SHAREDBUFFERWITHHEADER_HPP
#define BMBZ_TYPE_SHAREDBUFFERWITHHEADER_HPP

#include <boost/beast/core/flat_buffer.hpp>

namespace Bmbz
{
    namespace Type
    {
        class SharedBufferWithHeader
        {
        public:
            SharedBufferWithHeader() = default;
            SharedBufferWithHeader(std::size_t limitBody, std::size_t limitHeader = 16)
            : header(limitHeader)
            , body(limitBody)
            {}

            boost::beast::flat_buffer header;
            boost::beast::flat_buffer body;

            bool autoFree = true;
            bool busy = false;

            auto GetSendData() const
            {
                return std::array{header.data(), body.data()};
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
                header.consume(header.size());
                body.consume(body.size());
            }

        private:
            bool _sent = false;
        };
    }
}

#endif
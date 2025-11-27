#ifndef BMBZ_NETTYPE_NETWORKSTRING_HPP
#define BMBZ_NETTYPE_NETWORKSTRING_HPP

#include <string>
#include <cstdint>
#include "INetworkData.hpp"

namespace Bmbz 
{
namespace NetType
{
    struct NetworkString : public INetworkData
    {
        static const int maxLength = 60;

        NetworkString()
        {
            _content = "";
            _countBytes = 0;
        }

        NetworkString(std::string&& str)
        {
            Content(std::move(str));
        }

        NetworkString& operator=(const std::string& str)
        {
            Content(str);
            return *this;
        }

        NetworkString& operator=(std::string&& str)
        {
            Content(std::move(str));
            return *this;
        }

        bool operator==(const std::string& str) const
        {
            return _content == str;
        }

        const std::string& Content() const
        {
            return _content;
        }

        void Content(const std::string& newContent)
        {
            if (IsValid(newContent))
            {
                _content = newContent;
                _countBytes = static_cast<uint8_t>(_content.size());
            }
            else
            {
                throw "The length of the string is more than 60";
            }
        }

        void Content(std::string&& newContent)
        {
            if (IsValid(newContent))
            {
                _content = std::move(newContent);
                _countBytes = static_cast<uint8_t>(_content.size());
            }
            else
            {
                throw "The length of the string is more than 60";
            }
        }

        void Deserialize(boost::beast::flat_buffer& buffer) override
        {
            Utils::ReadFromBuffer(buffer, _countBytes);
            _content.assign(static_cast<const char*>(buffer.cdata().data()), _countBytes);
            buffer.consume(_countBytes);
        }

        void Serialize(boost::beast::flat_buffer& buffer) const override
        {
            Utils::WriteToBuffer(buffer, _countBytes);
            boost::asio::buffer_copy(buffer.prepare(_countBytes), boost::asio::buffer(_content));
            buffer.commit(_countBytes);
        }

    private:
        std::string _content;
        uint8_t _countBytes;

        bool IsValid(const std::string& str)
        {
            return str.length() <= maxLength;
        }
    };
}
}
#endif
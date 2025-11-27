// #ifndef BMBZ_WRAPNETWORKARRAY256_HPP
// #define BMBZ_WRAPNETWORKARRAY256_HPP

// #include <vector>
// #include "Utils/ReadWriteData.hpp"

// namespace Bmbz
// {
// namespace NetType
// {
//     template<typename T>
//     struct WrapNetworkArray256
//     {
//         static constexpr size_t maxLength = 255;

//         WrapNetworkArray256(std::vector<T>& vector) : _values(vector)
//         {
//             if (vector.size() > maxLength)
//                 throw "NetworkList256: The length of the list must be less than or equal to 255";
//         }
    
//         const std::vector<T>& GetVector()
//         {
//             return _values;
//         }
    
//         void Serialize(boost::beast::flat_buffer& buffer) const
//         {
//             Utils::WriteToBuffer(buffer, static_cast<uint8_t>(_values.size()));
//             for (const auto& v : _values)
//             {
//                 Utils::WriteToBuffer(buffer, v);
//             }
//         }

//         void Deserialize(boost::beast::flat_buffer& buffer)
//         {
//             uint8_t length;
//             Utils::ReadFromBuffer(buffer, length);
//             _values.reserve(length);

//             for (size_t i = 0; i < length; i++)
//             {
//                 Utils::ReadFromBuffer(buffer, _values[i]);
//             }
//         }
    
//     private:
//         std::vector<T>& _values;
//     };
// }
// }
// #endif
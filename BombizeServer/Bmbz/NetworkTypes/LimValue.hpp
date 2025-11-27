// #ifndef BMBZ_NETTYPE_LIMVALUE_HPP
// #define BMBZ_NETTYPE_LIMVALUE_HPP

// #include "Interfaces/INetworkData.hpp"
// #include "Utils/DataConversion.hpp"

// namespace Bmbz
// {
//     namespace NetType
//     {
//         template<typename T>
//         struct LimValue : Interface::INetworkData
//         {
//             T minValue, maxValue;

//             LimValue(const T& minValue, const T& maxValue, const T& value)
//             : minValue(minValue)
//             , maxValue(maxValue)
//             {
//                 Value(value);
//             }

//             virtual ~LimValue() = default;

//             const T& Value() const
//             {
//                 return _value;
//             }

//             void Value(const T& value)
//             {
//                 if (IsValid(value))
//                     _value = value;
//                 else if (_value < minValue)
//                     _value = minValue;
//                 else if (_value > maxValue)
//                     _value = maxValue;
//             }

//             bool IsValid()
//             {
//                 return IsValid(_value);
//             }

//             void Serialize(boost::beast::flat_buffer& buffer) const override
//             {
//                 Utils::WriteToBuffer(buffer, _value);
//             }

//             void Deserialize(boost::beast::flat_buffer& buffer) override
//             {
//                 Utils::ReadFromBuffer(buffer, _value);
//                 Value(_value);
//             }

//         private:
//             T _value;

//             bool IsValid(const T& value)
//             {
//                 return minValue <= value && value <= maxValue;
//             }
//         };


//         template<typename FixedType>
//         struct LimValueFixed : LimValue<float>
//         {
//             LimValueFixed(
//                 const float& minValue, 
//                 const float& maxValue, 
//                 const float& value,
//                 const float& precision
//             )
//             : LimValue<float>(minValue, maxValue, value)
//             , _precision(precision)
//             {}

//             void Serialize(std::ostream& toStream) const override
//             {
//                 Utils::WriteToStream(
//                     toStream,
//                     Utils::ConvertToFixed<FixedType>(Value(), minValue, _precision)
//                 );
//             }

//             void Deserialize(boost::beast::flat_buffer& buffer) override
//             {
//                 FixedType fixedVar;
//                 Utils::ReadFromBuffer(buffer, fixedVar);
//                 Value(Utils::ConvertFromFixed<FixedType>(fixedVar, minValue, _precision));
//             }

//         private:
//             float _precision;
//         };
//     }
// }

// #endif
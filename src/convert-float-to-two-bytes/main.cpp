#include <array>
#include <iostream>
#include <memory>
#include <bitset>
#include <cstring>
#include <cstddef>

constexpr float LSB_VALUE{4.8E-3F};

std::array<std::byte, 2> float_to_bytes(float value)
{
    std::array<std::byte, 2> two_byte_array{};
    const float scaled_value{value / LSB_VALUE};
    const std::int16_t scaled_value_int{static_cast<std::int16_t>(scaled_value)};
    std::memcpy(two_byte_array.data(), &scaled_value_int, 2);

    return two_byte_array;
}

float bytes_to_float(const std::array<std::byte, 2> &byte_array)
{
    std::cout << " int8_t: " << std::bitset<8>(std::int8_t()) << " uint8_t: " << std::bitset<8>(std::uint8_t()) << std::endl;
    std::int16_t scaled_value_int{};
    std::memcpy(&scaled_value_int, byte_array.data(), 2);
    std::cout << "scaled_value_int: " << scaled_value_int << " (bits): " << std::bitset<16>(scaled_value_int) << std::endl;
    const float scaled_value{static_cast<float>(scaled_value_int)};

    return scaled_value * LSB_VALUE;
}

int main()
{
    const float a_x{LSB_VALUE};
    const auto encoded = float_to_bytes(a_x);

    std::cout << "encoded byte: " << std::bitset<8>(std::to_integer<std::int8_t>(encoded[0])) << std::bitset<8>(std::to_integer<std::int8_t>(encoded[1])) << std::endl;
    std::cout << "Deconding: " << std::endl;
    std::cout << "decoded float" << bytes_to_float(encoded) << std::endl;
    std::bitset<8> b1{0xb0000101};
    std::array<std::byte, 2> two_byte_array{{static_cast<std::byte>(0x1), static_cast<std::byte>(0x0b)}};
    std::cout << b1 << std::endl;
    return 0;
}

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstdint>

namespace transplant {

// Record types
constexpr uint8_t START_OF_TRANSMISSION = 0;
constexpr uint8_t END_OF_TRANSMISSION = 1;
constexpr uint8_t START_OF_DIRECTORY = 2;
constexpr uint8_t END_OF_DIRECTORY = 3;
constexpr uint8_t DIRECTORY_ENTRY = 4;
constexpr uint8_t FILE_DATA = 5;

constexpr uint32_t MAGIC = 0x0C0DED;

// Options flags
constexpr int HELP_FLAG = 0x1;
constexpr int SERIALIZE_FLAG = 0x2;
constexpr int DESERIALIZE_FLAG = 0x4;
constexpr int CLOBBER_FLAG = 0x8;

} // namespace transplant

#endif

#ifndef BINARY_IO_HPP
#define BINARY_IO_HPP

#include <cstdio>
#include <cstdint>

namespace transplant {

struct RecordHeader {
    uint32_t magic;
    uint8_t type;
    uint32_t depth;
    uint64_t size;
};

class BinaryIO {
public:
    BinaryIO() = default;
    ~BinaryIO() = default;

    // Check if an I/O error occurred
    bool has_error() const { return m_error_flag; }

    // Reset error flag
    void reset_error() { m_error_flag = false; }

    // Write 32-bit unsigned integer in big-endian format
    void write_u32_be(uint32_t value, std::FILE* out);

    // Write 64-bit unsigned integer in big-endian format
    void write_u64_be(uint64_t value, std::FILE* out);

    // Read 32-bit unsigned integer in big-endian format
    uint32_t read_u32_be(std::FILE* in);

    // Read 64-bit unsigned integer in big-endian format
    uint64_t read_u64_be(std::FILE* in);

    // Write record header
    void write_header(std::FILE* out, uint8_t type, uint32_t depth, uint64_t size);

    // Read record header
    int read_header(std::FILE* in, RecordHeader* header);

private:
    void check_io_error(std::FILE* f);

    bool m_error_flag = false;
};

} // namespace transplant

#endif

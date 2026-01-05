#include "binary_io.hpp"
#include "constants.hpp"
#include <cstdio>

namespace transplant {

void BinaryIO::check_io_error(std::FILE* f) {
    if (std::ferror(f) || std::feof(f)) {
        m_error_flag = true;
    }
}

void BinaryIO::write_u32_be(uint32_t value, std::FILE* out) {
    std::fputc((value >> 24) & 0xFF, out);
    std::fputc((value >> 16) & 0xFF, out);
    std::fputc((value >> 8) & 0xFF, out);
    std::fputc(value & 0xFF, out);

    check_io_error(out);
}

void BinaryIO::write_u64_be(uint64_t value, std::FILE* out) {
    std::fputc((value >> 56) & 0xFF, out);
    std::fputc((value >> 48) & 0xFF, out);
    std::fputc((value >> 40) & 0xFF, out);
    std::fputc((value >> 32) & 0xFF, out);
    std::fputc((value >> 24) & 0xFF, out);
    std::fputc((value >> 16) & 0xFF, out);
    std::fputc((value >> 8) & 0xFF, out);
    std::fputc(value & 0xFF, out);

    check_io_error(out);
}

uint32_t BinaryIO::read_u32_be(std::FILE* in) {
    uint32_t value = 0;

    value |= std::fgetc(in) << 24;
    value |= std::fgetc(in) << 16;
    value |= std::fgetc(in) << 8;
    value |= std::fgetc(in);

    check_io_error(in);

    return value;
}

uint64_t BinaryIO::read_u64_be(std::FILE* in) {
    uint64_t value = 0;

    value |= static_cast<uint64_t>(std::fgetc(in)) << 56;
    value |= static_cast<uint64_t>(std::fgetc(in)) << 48;
    value |= static_cast<uint64_t>(std::fgetc(in)) << 40;
    value |= static_cast<uint64_t>(std::fgetc(in)) << 32;
    value |= static_cast<uint64_t>(std::fgetc(in)) << 24;
    value |= static_cast<uint64_t>(std::fgetc(in)) << 16;
    value |= static_cast<uint64_t>(std::fgetc(in)) << 8;
    value |= static_cast<uint64_t>(std::fgetc(in));

    check_io_error(in);

    return value;
}

void BinaryIO::write_header(std::FILE* out, uint8_t type, uint32_t depth, uint64_t size) {
    std::fputc((MAGIC >> 16) & 0xFF, out);
    std::fputc((MAGIC >> 8) & 0xFF, out);
    std::fputc(MAGIC & 0xFF, out);

    std::fputc(type, out);
    write_u32_be(depth, out);
    write_u64_be(size, out);

    check_io_error(out);
}

int BinaryIO::read_header(std::FILE* in, RecordHeader* header) {
    header->magic = 0;
    header->magic |= std::fgetc(in) << 16;
    header->magic |= std::fgetc(in) << 8;
    header->magic |= std::fgetc(in);

    check_io_error(in);

    if (header->magic != MAGIC) {
        return -1;
    }

    header->type = std::fgetc(in);
    header->depth = read_u32_be(in);
    header->size = read_u64_be(in);

    if (has_error()) {
        return -1;
    }

    return 0;
}

} // namespace transplant

#include "deserializer.hpp"
#include "constants.hpp"
#include <sys/stat.h>
#include <cstdio>

namespace transplant {

Deserializer::Deserializer(PathManager& path_manager, BinaryIO& io, bool clobber)
    : m_path_manager(path_manager), m_io(io), m_clobber(clobber) {}

int Deserializer::deserialize() {
    RecordHeader header;

    if (m_io.read_header(stdin, &header) || header.type != START_OF_TRANSMISSION || header.depth) {
        return -1;
    }

    struct stat stat_buf;
    if (stat(m_path_manager.get_path(), &stat_buf)) {
        if (mkdir(m_path_manager.get_path(), 0700)) {
            return -1;
        }
    } else if (!S_ISDIR(stat_buf.st_mode)) {
        return -1;
    }

    if (deserialize_directory(1)) {
        return -1;
    }

    if (m_io.read_header(stdin, &header) || header.type != END_OF_TRANSMISSION || header.depth) {
        return -1;
    }

    return 0;
}

int Deserializer::deserialize_directory(int depth) {
    RecordHeader header;
    uint32_t u_depth = static_cast<uint32_t>(depth);

    if (m_io.read_header(stdin, &header) || header.type != START_OF_DIRECTORY || header.depth != u_depth) {
        return -1;
    }

    while (true) {
        if (m_io.read_header(stdin, &header)) {
            return -1;
        }

        if (header.type == END_OF_DIRECTORY && header.depth == u_depth) {
            break;
        }

        if (header.type != DIRECTORY_ENTRY || header.depth != u_depth) {
            return -1;
        }

        uint32_t mode = m_io.read_u32_be(stdin);
        uint64_t size = m_io.read_u64_be(stdin);
        if (m_io.has_error()) {
            return -1;
        }

        int de_length = header.size - (16 + 12);
        int c;
        for (int i = 0; i < de_length; i++) {
            if ((c = std::fgetc(stdin)) == EOF) {
                return -1;
            }
            *(m_path_manager.get_name_buf() + i) = c;
        }
        *(m_path_manager.get_name_buf() + de_length) = '\0';

        if (m_path_manager.push(m_path_manager.get_name_buf())) {
            return -1;
        }

        struct stat stat_buf;
        if (S_ISDIR(mode)) {
            if (!stat(m_path_manager.get_path(), &stat_buf)) {
                // Directory already exists
            } else {
                if (mkdir(m_path_manager.get_path(), 0700)) {
                    return -1;
                }
            }

            if (deserialize_directory(depth + 1)) {
                return -1;
            }

            if (chmod(m_path_manager.get_path(), mode & 0777)) {
                return -1;
            }
        } else if (S_ISREG(mode)) {
            if (deserialize_file(depth + 1)) {
                return -1;
            }
        }

        if (m_path_manager.pop()) {
            return -1;
        }
    }

    if (std::ferror(stdin)) {
        return -1;
    }

    return 0;
}

int Deserializer::deserialize_file(int depth) {
    RecordHeader header;
    uint32_t u_depth = static_cast<uint32_t>(depth);

    if (m_io.read_header(stdin, &header) || header.type != FILE_DATA || header.depth != u_depth) {
        return -1;
    }

    struct stat file_stat;
    if (!stat(m_path_manager.get_path(), &file_stat) && !m_clobber) {
        return -1;
    }

    std::FILE* f = std::fopen(m_path_manager.get_path(), "w");
    if (!f) {
        return -1;
    }

    int c;
    uint64_t file_size = header.size - 16;
    for (uint64_t i = 0; i < file_size; i++) {
        if ((c = std::fgetc(stdin)) == EOF) {
            std::fclose(f);
            return -1;
        }
        if (std::fputc(c, f) == EOF) {
            std::fclose(f);
            return -1;
        }
    }

    if (std::ferror(f) || std::ferror(stdin)) {
        std::fclose(f);
        return -1;
    }

    std::fclose(f);
    return 0;
}

} // namespace transplant

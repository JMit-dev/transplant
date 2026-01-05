#include "serializer.hpp"
#include "constants.hpp"
#include <dirent.h>
#include <sys/stat.h>
#include <cstdio>

namespace transplant {

Serializer::Serializer(PathManager& path_manager, BinaryIO& io)
    : m_path_manager(path_manager), m_io(io) {}

int Serializer::serialize() {
    m_io.write_header(stdout, START_OF_TRANSMISSION, 0, 16);
    if (m_io.has_error()) {
        return -1;
    }

    if (serialize_directory(1)) {
        return -1;
    }

    m_io.write_header(stdout, END_OF_TRANSMISSION, 0, 16);
    if (m_io.has_error()) {
        return -1;
    }

    std::fflush(stdout);
    return 0;
}

int Serializer::serialize_directory(int depth) {
    DIR* dir = opendir(m_path_manager.get_path());
    if (!dir) {
        return -1;
    }

    m_io.write_header(stdout, START_OF_DIRECTORY, depth, 16);
    if (m_io.has_error()) {
        closedir(dir);
        return -1;
    }

    struct dirent* de;
    struct stat stat_buf;

    while ((de = readdir(dir))) {
        // Skip . and ..
        if (*(de->d_name) == '.' && (*(de->d_name + 1) == '\0'
            || (*(de->d_name + 1) == '.' && *(de->d_name + 2) == '\0'))) {
            continue;
        }

        int de_length = 0;
        while (*(de->d_name + de_length) != '\0') {
            de_length++;
        }

        if (m_path_manager.push(de->d_name)) {
            closedir(dir);
            return -1;
        }

        if (stat(m_path_manager.get_path(), &stat_buf)) {
            m_path_manager.pop();
            closedir(dir);
            return -1;
        }

        if (S_ISREG(stat_buf.st_mode)) {
            uint64_t entry_size = 16 + 12 + de_length;
            m_io.write_header(stdout, DIRECTORY_ENTRY, depth, entry_size);
            m_io.write_u32_be(stat_buf.st_mode, stdout);
            m_io.write_u64_be(stat_buf.st_size, stdout);
            if (m_io.has_error()) {
                m_path_manager.pop();
                closedir(dir);
                return -1;
            }
            for (int i = 0; i < de_length; i++) {
                if (std::fputc(*(de->d_name + i), stdout) == EOF) {
                    m_path_manager.pop();
                    closedir(dir);
                    return -1;
                }
            }
            if (serialize_file(depth + 1, stat_buf.st_size)) {
                m_path_manager.pop();
                closedir(dir);
                return -1;
            }
        } else if (S_ISDIR(stat_buf.st_mode)) {
            uint64_t entry_size = 16 + 12 + de_length;
            m_io.write_header(stdout, DIRECTORY_ENTRY, depth, entry_size);
            m_io.write_u32_be(stat_buf.st_mode, stdout);
            m_io.write_u64_be(0, stdout);
            if (m_io.has_error()) {
                m_path_manager.pop();
                closedir(dir);
                return -1;
            }

            for (int i = 0; i < de_length; i++) {
                if (std::fputc(*(de->d_name + i), stdout) == EOF) {
                    m_path_manager.pop();
                    closedir(dir);
                    return -1;
                }
            }

            if (serialize_directory(depth + 1)) {
                m_path_manager.pop();
                closedir(dir);
                return -1;
            }
        }

        if (m_path_manager.pop()) {
            closedir(dir);
            return -1;
        }
    }

    m_io.write_header(stdout, END_OF_DIRECTORY, depth, 16);
    if (m_io.has_error()) {
        closedir(dir);
        return -1;
    }
    closedir(dir);

    if (std::ferror(stdout)) {
        return -1;
    }

    return 0;
}

int Serializer::serialize_file(int depth, off_t size) {
    std::FILE* f = std::fopen(m_path_manager.get_path(), "r");
    if (!f) {
        return -1;
    }

    uint64_t record_size = 16 + size;
    m_io.write_header(stdout, FILE_DATA, depth, record_size);
    if (m_io.has_error()) {
        std::fclose(f);
        return -1;
    }

    int c;
    for (off_t i = 0; i < size; i++) {
        if ((c = std::fgetc(f)) == EOF) {
            std::fclose(f);
            return -1;
        }
        if (std::fputc(c, stdout) == EOF) {
            std::fclose(f);
            return -1;
        }
    }

    if (std::ferror(f) || std::ferror(stdout)) {
        std::fclose(f);
        return -1;
    }

    std::fclose(f);
    return 0;
}

} // namespace transplant

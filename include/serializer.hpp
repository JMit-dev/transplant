#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include "path_manager.hpp"
#include "binary_io.hpp"
#include <cstdio>

namespace transplant {

class Serializer {
public:
    Serializer(PathManager& path_manager, BinaryIO& io);
    ~Serializer() = default;

    // Serialize entire tree
    int serialize();

private:
    // Serialize directory contents
    int serialize_directory(int depth);

    // Serialize single file
    int serialize_file(int depth, off_t size);

    PathManager& m_path_manager;
    BinaryIO& m_io;
};

} // namespace transplant

#endif

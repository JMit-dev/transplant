#ifndef DESERIALIZER_HPP
#define DESERIALIZER_HPP

#include "path_manager.hpp"
#include "binary_io.hpp"
#include <cstdio>

namespace transplant {

class Deserializer {
public:
    Deserializer(PathManager& path_manager, BinaryIO& io, bool clobber);
    ~Deserializer() = default;

    // Deserialize entire tree
    int deserialize();

    // Deserialize directory contents
    int deserialize_directory(int depth);

    // Deserialize single file
    int deserialize_file(int depth);

private:
    PathManager& m_path_manager;
    BinaryIO& m_io;
    bool m_clobber;
};

} // namespace transplant

#endif

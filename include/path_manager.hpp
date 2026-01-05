#ifndef PATH_MANAGER_HPP
#define PATH_MANAGER_HPP

#include <string>
#include <climits>

namespace transplant {

class PathManager {
public:
    PathManager() = default;
    ~PathManager() = default;

    // Initialize path with a base path
    int init(const char* name);

    // Append a component to the path
    int push(const char* name);

    // Remove the last component from the path
    int pop();

    // Get current path as C string
    const char* get_path() const { return m_path_buf; }

    // Get current path length
    int get_length() const { return m_path_length; }

    // Get name buffer for reading directory entries
    char* get_name_buf() { return m_name_buf; }

private:
    char m_path_buf[PATH_MAX] = {0};
    char m_name_buf[NAME_MAX] = {0};
    int m_path_length = 0;
};

} // namespace transplant

#endif

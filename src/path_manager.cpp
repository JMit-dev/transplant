#include "path_manager.hpp"
#include <cstring>

namespace transplant {

int PathManager::init(const char* name) {
    m_path_length = 0;

    while (*name != '\0') {
        if (m_path_length >= PATH_MAX - 1) {
            return -1;
        }
        *(m_path_buf + m_path_length) = *name;
        name++;
        m_path_length++;
    }
    *(m_path_buf + m_path_length) = '\0';
    return 0;
}

int PathManager::push(const char* name) {
    int name_length = 0;

    while (*(name + name_length) != '\0') {
        if (*(name + name_length) == '/') {
            return -1;
        }
        name_length++;
    }

    if (m_path_length + name_length + 1 > PATH_MAX - 1) {
        return -1;
    }

    *(m_path_buf + m_path_length) = '/';
    m_path_length++;

    for (int i = 0; i < name_length; i++) {
        *(m_path_buf + m_path_length) = *(name + i);
        m_path_length++;
    }
    *(m_path_buf + m_path_length) = '\0';

    return 0;
}

int PathManager::pop() {
    if (m_path_length == 0) {
        return -1;
    }

    int i_separator = m_path_length - 1;
    while (i_separator >= 0 && *(m_path_buf + i_separator) != '/') {
        i_separator--;
    }

    if (i_separator < 0) {
        m_path_length = 0;
        *m_path_buf = '\0';
        return 0;
    }

    m_path_length = i_separator;
    *(m_path_buf + m_path_length) = '\0';
    return 0;
}

} // namespace transplant

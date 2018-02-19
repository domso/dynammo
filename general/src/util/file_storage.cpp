#include "src/util/file_storage.h"

util::file_storage::file_storage() {
    
}

util::file_storage::~file_storage() {
    
}

bool util::file_storage::init(const std::string& filename) {
    m_file.open(filename);
    
    return m_file.is_open();
}


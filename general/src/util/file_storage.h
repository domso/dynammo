#ifndef util_file_storage_h
#define util_file_storage_h

#include <string>
#include <fstream>

namespace util {
    class file_storage {
    public:
        file_storage();
        file_storage(const file_storage& copy) = delete;        
        ~file_storage();
        
        bool init(const std::string& filename);
        
        
        template <typename T>
        bool write(const T* input, const int n = 1) {
            if (m_file.is_open()) {
                m_file.write((char*)input, n * sizeof(T));
                return m_file.good();
            }
            
            return false;
        }
        
        template <typename T>
        bool read(const T* input, const int n = 1) {
            if (m_file.is_open()) {
                m_file.read((char*)input, n * sizeof(T));
                return m_file.good();
            }
            
            return false;
        }
        
        
    private:
        std::fstream m_file;
    };
}

#endif

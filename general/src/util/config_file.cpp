#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "src/util/config_file.h"

void util::config_file::load(const std::string& filename) {
    std::lock_guard<std::mutex> lg(m_mutex);
    std::ifstream file;
    std::string line;

    file.open(filename);

    if (file.is_open()) {
        while (std::getline(file, line)) {
            insert_new_pair(line);
        }

        file.close();
    }
}

void util::config_file::store(const std::string& filename) {
    std::lock_guard<std::mutex> lg(m_mutex);
    std::ofstream file;
    std::string line;

    file.open(filename);

    if (file.is_open()) {
        for (auto& item : m_configMap) {
            file << item.first << ": " << item.second << "\n";
        }

        file.close();
    }
}

void util::config_file::clear() {
    std::lock_guard<std::mutex> lg(m_mutex);
    m_configMap.clear();
}

bool util::config_file::insert_new_pair(const std::string input) {    
    std::string clearedInput = remove_whitespace(input);
    auto position = clearedInput.find(":");    
    
    if (position != std::string::npos) {        
        m_configMap.insert({clearedInput.substr(0, position), clearedInput.substr(position + 1)});
        return true;
    }
    
    return false;
}

std::string util::config_file::remove_whitespace(const std::string input) {
    std::string result = input;
    result.erase(std::remove(result.begin(), result.end(), ' '));
    return result;
}

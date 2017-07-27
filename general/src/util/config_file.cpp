#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include "src/util/config_file.h"

namespace util {

    config_file::config_file() {

    }

    config_file::~config_file() {

    }

    void config_file::load(std::string filename) {
        std::ifstream file;
        std::string line;

        file.open(filename);

        if (file.is_open()) {
            while (std::getline(file, line)) {
                insert(line);
            }

            file.close();

            m_filename = filename;
            return;
        }

        m_filename = "";
        m_errorMsg += "[ERROR] Could not open '" + filename + "' as config file.";
        return;
    }

    void config_file::requireString(const std::string& key) {
        if (m_filename.empty()) {
            return;
        }

        auto result = m_map.find(key);

        if (result != m_map.end()) {
            if (result->second != "") {
                return;
            }
        }

        m_errorMsg += "[ERROR] Require '" + key + "'";
        m_errorMsg += " in " + m_filename + ".\n";
    }

    void config_file::recommendString(const std::string& key, const std::string& defaultValue) {
        if (m_filename.empty()) {
            return;
        }

        auto result = m_map.find(key);

        if (result != m_map.end()) {
            if (result->second != "") {
                return;
            }
        }

        m_map[key] = defaultValue;
        m_warningMsg += "[WARNING] Recommend '" + key + "'";
        m_warningMsg += " (Default = '" + defaultValue + "')";
        m_warningMsg += " in " + m_filename + ".\n";
    }

    std::string config_file::getString(const std::__cxx11::string& key, const std::__cxx11::string& defaultValue) const {
        auto result = m_map.find(key);

        if (result != m_map.end()) {
            if (result->second != "") {
                return result->second;
            }
        }

        return defaultValue;
    }

    bool config_file::isError() const {
        return !m_errorMsg.empty();
    }

    const std::string& config_file::getErrorMsgs() const {
        return m_errorMsg;
    }

    bool config_file::isWarning() const {
        return !m_warningMsg.empty();
    }

    const std::string& config_file::getWarningMsgs() const {
        return m_warningMsg;
    }

    void config_file::clear() {
        m_errorMsg = "";
        m_warningMsg = "";
    }

    bool config_file::insert(const std::string input) {
        int mode = 0;
        int num_chars[2];
        num_chars[0] = 0;
        num_chars[1] = 0;

        for (int i = 0; i < input.length(); i++) {
            if (input[i] == ':') {
                mode = 1;
            } else if (input[i] != ' ') {
                num_chars[mode]++;
            }
        }

        if (mode == 0 || num_chars[0] == 0 || num_chars[1] == 0) {
            return false;
        }

        std::string components[2];

        components[0].resize(num_chars[0], ' ');
        components[1].resize(num_chars[1], ' ');

        mode = 0;
        num_chars[0] = 0;
        num_chars[1] = 0;

        for (int i = 0; i < input.length(); i++) {
            if (input[i] == ':') {
                mode = 1;
            } else if (input[i] != ' ') {
                components[mode][num_chars[mode]] = input[i];
                num_chars[mode]++;
            }
        }

        if (components[0][0] != '#') {
            m_map.insert({components[0], components[1]});
        }

        return true;
    }

}


#ifndef general_util_config_file_h
#define general_util_config_file_h

#include <string>
#include <unordered_map>
#include <type_traits>

namespace util {
    /**
    * @brief simple class to load, parse, validate and access a configuration file
    * 
    */
    class config_file {
    public:
        /**
        * @brief tries to open the file specified by filename;
        * every line will be parsed into a key-value pair with ':' as seperator;
        * empty space will be ignored
        * 
        * @param filename the absolut or relative path to the config file
        */
        void load(const std::string& filename);
        
        /**
        * @brief tries to store all current data in the file specified by filename;
        * every line will be parsed into a key-value pair with ':' as seperator;
        * 
        * @param filename the absolut or relative path to the config file
        */
        void store(const std::string& filename);
               
        
        template <typename T>
        std::pair<bool, T> operator=(const std::string& key) {
            return get<T>(key);
        }
        
        /**
        * @brief gets the stored value for the given key, or inserts the given defaultValue as the new value
        * 
        * @param T any arithmetic type or std::string
        * @param key the data-key
        * @param defaultValue defaultValue for inserts
        * @return {no insertion, storedValue}
        */ 
        template <typename T>
        typename std::enable_if<std::is_arithmetic<T>::value, std::pair<bool, T>>::type get(const std::string& key, const T defaultValue = T()) {
            auto result = convert_string<T>(m_configMap[key]);
            if (!result.first) {          
                m_configMap[key] = std::to_string(defaultValue);
                return std::pair<bool, T>(false, defaultValue);
            }         
            
            return std::pair<bool, T>(true, result.second);
        }
        
        /**
        * @brief gets the stored value for the given key, or inserts the given defaultValue as the new value
        * 
        * @param T any arithmetic type or std::string
        * @param key the data-key
        * @param defaultValue defaultValue for inserts
        * @return {no insertion, storedValue}
        */ 
        template <typename T>
        typename std::enable_if<std::is_same<T, std::string>::value, std::pair<bool, T>>::type get(const std::string& key, const T defaultValue = T()) {
            auto result = m_configMap[key];
            if (result == "") {      
                m_configMap[key] = defaultValue;
                return std::pair<bool, T>(false, defaultValue);
            }
            
            return std::pair<bool, T>(true, result);
        }
        
        /**
        * @brief overrides any conflicting configuration with the given key-value pair
        * 
        * @param T anything working with std::to_string
        * @param key 
        * @param value 
        */
        template <typename T>
        typename std::enable_if<std::is_same<std::string, T>::value, void>::type set(const std::string& key, const T& value) {
            m_configMap[key] = value;
        }
        template <typename T>
        typename std::enable_if<not std::is_same<std::string, T>::value, void>::type set(const std::string& key, const T& value) {
            m_configMap[key] = std::to_string(value);                            
        }
        
        /**
        * @brief clears all key-value pairs
        * 
        */
        void clear();        
    private:                
        template <typename T>
        std::pair<bool, T> convert_string(const std::string& key) const {            
            char* endPtr = nullptr;
            const char* cPtr = key.c_str();
            T result;
            bool castFailure = false;
            
            if (std::is_integral<T>::value) {
                if (std::is_signed<T>::value) {
                    auto tmp = strtoll(cPtr, &endPtr, 10);
                    // tmp is too large for T, than a comparision will fail
                    result = tmp;
                    castFailure = result != tmp;
                } else {
                    auto tmp = strtoull(cPtr, &endPtr, 10);
                    result = tmp;
                    castFailure = result != tmp;
                }
            }
            
            if (std::is_floating_point<T>::value) {
                auto tmp = strtold(cPtr, &endPtr);
                result = tmp;
                castFailure = result != tmp;
            }
                                
            return std::pair<bool, T>(!castFailure && endPtr != cPtr, result);
        }
                
        bool insert_new_pair(const std::string input);
        std::string remove_whitespace(const std::string input);

        std::unordered_map<std::string, std::string> m_configMap;
    };
}

#endif


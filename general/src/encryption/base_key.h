#pragma once

#include <stdint.h>
#include <string>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>

namespace encryption {   
    /**
    * @brief Abstract rsa-key wrapper
    * 
    */
    class base_key {
    public:
        base_key();
        ~base_key();
        
        base_key(const base_key& copy) = delete;
        base_key(base_key&& move);
        base_key(RSA* rsa);
        void operator=(base_key&& copy);
        
        /**
        * @brief Returns the internal RSA-instance
        * 
        * @return RSA*
        */
        RSA* getRSA() const;
        
        /**
        * @brief Returns true, if the object holds an internal key
        * 
        * @return bool
        */
        bool valid() const;
        
        /**
        * @brief Loads rsa keys
        * 
        * @param filename rsa-key in pem-format
        * @return Success
        */
        bool load(const std::string filename);
        
        /**
        * @brief Stores rsa keys
        * 
        * @param filename rsa-key in pem-format
        * @return Success
        */
        bool save(const std::string filename);
        
        /**
        * @brief Returns the required size for encryptChar(), decryptChar(), signChar() and verifyChar()
        * 
        * @return int
        */
        int required_size() const;
    protected:
        virtual bool load_RSA(FILE* file) = 0;        
        virtual bool save_RSA(BIO* file, EVP_PKEY* pkey) = 0;
        RSA* m_rsa;
        int m_requiredSize;
    };
}

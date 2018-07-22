#pragma once

#include "src/encryption/base_key.h"

namespace encryption {
    /**
    * @brief Wrapper for public-key
    *
    */
    class public_key : public base_key {
    public:
        /**
        * @brief  tries to encrypt dataLen bytes from 'in' with the public key and store the encrypted bytes into 'out'
        * - for encryption RSA_public_encrypt() with RSA_PKCS1_OAEP_PADDING is used
        * - the required size for the output-buffer will be checked
        * - the input will be splitted into "key.getRequiredSize"-42 input-blocks
        * - each input-block will be encrypted to a "key.getRequiredSize()" large output block
        *
        * @param dataLen length of the input buffer
        * @param in input buffer
        * @param bufferLen length of the output buffer
        * @param out output buffer
        * @return int
        */
        int encryptChar(const int dataLen, const uint8_t* in, const int bufferLen, uint8_t* out);
        
        /**
        * @brief tries to validate n instances of T from 'in' with the public key
        * - for validation RSA_public_decrypt() with SHA1 and RSA_PKCS1_PADDING is used
        * 
        * @param T Input typename
        * @param signature pointer to the signature
        * @param signatureLength length of the signature
        * @param in input buffer of type T
        * @param n number of elemens in the input buffer
        * @return success
        */
        template <typename T>
        bool verify(const uint8_t* signature, const int signatureLength, const T* in, const int n = 1) {
            uint8_t hash[SHA256_DIGEST_LENGTH];
            SHA256((uint8_t*)in, sizeof(T) * n, hash);

            uint8_t calcSignatureBuffer[required_size()];

            if (RSA_public_decrypt(signatureLength, signature, calcSignatureBuffer, getRSA(), RSA_PKCS1_PADDING) == SHA256_DIGEST_LENGTH) {
                return memcmp(hash, calcSignatureBuffer, SHA256_DIGEST_LENGTH) == 0;
            }

            return false;
        }
    private:
        using base_key::base_key;
        bool load_RSA(FILE* file);
        bool save_RSA(BIO* file, EVP_PKEY* pkey);
    };
}

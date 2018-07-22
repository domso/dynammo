#pragma once

#include "src/encryption/base_key.h"

namespace encryption {
    /**
    * @brief Wrapper for pivate-key
    *
    */
    class private_key : public base_key {
    public:
        /**
        * @brief tries to decrypt dataLen bytes from 'in' with the private key and store the decrypted bytes into 'out'
        * - for decryption RSA_private_decrypt() with RSA_PKCS1_OAEP_PADDING is used
        * - the required size for the output-buffer will be checked
        * - the input will be splitted into "key.getRequiredSize" input-blocks
        * - each input-block will be decrypted to a "key.getRequiredSize()" large output block
        *
        * @param dataLen size of input buffer
        * @param in input buffer
        * @param bufferLen size of output buffer
        * @param out output buffer
        * @return int
        */
        int decryptChar(int dataLen, const uint8_t* in, const int bufferLen, uint8_t* out);
        
        /**
        * @brief - tries to sign n instances of T from 'in' with the private key and store the signature into 'signature'
        * - for signing RSA_private_encrypt() with SHA1 and RSA_PKCS1_PADDING is used
        * - the required size for the output-buffer will be checked
        *
        * @param T input buffer type
        * @param signature signature
        * @param signatureLength length of signature
        * @param in input buffer of type T
        * @param n number of elements in the input buffer
        * @return int
        */
        template <typename T>
        int sign(uint8_t* signature, const int signatureLength, const T* in, const int n = 1) {
            int result;
            uint8_t hash[SHA256_DIGEST_LENGTH];

            if (signatureLength < required_size()) {
                return -1;
            }

            SHA256((uint8_t*)in, sizeof(T) * n, hash);
            result = RSA_private_encrypt(SHA256_DIGEST_LENGTH, hash, signature, getRSA(), RSA_PKCS1_PADDING);

            if (result < 0) {
                return 0;
            }

            return result;
        }
    private:
        using base_key::base_key;
        bool load_RSA(FILE* file);
        bool save_RSA(BIO* file, EVP_PKEY* pkey);
    };
}

#include <iostream>
#include <stdio.h>
#include <cstring>
#include <assert.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include "src/encryption/rsa.h"

namespace encryption {

    base_key::base_key() {
        m_rsa = nullptr;
        m_requiredSize = 0;
    }

    base_key::~base_key() {
        if (m_rsa != nullptr) {
            RSA_free(m_rsa);
        }
    }

    RSA* base_key::getRSA() const {
        return m_rsa;
    }

    bool base_key::load(const std::string filename) {
        FILE* file = fopen(filename.c_str(), "rb");

        if (file != nullptr) {
            bool result = setRSA(file);
            fclose(file);
            m_requiredSize = RSA_size(m_rsa);
            return result;
        }

        return false;
    }

    int base_key::getRequiredSize() const {
        return m_requiredSize;
    }

    bool private_key::setRSA(FILE* file) {
        m_rsa = PEM_read_RSAPrivateKey(file, nullptr, nullptr, nullptr);
        return m_rsa != nullptr;
    }

    bool public_key::setRSA(FILE* file) {
        m_rsa = PEM_read_RSA_PUBKEY(file, nullptr, nullptr, nullptr);
        return m_rsa != nullptr;
    }

    int encryptChar(const public_key& key, const int dataLen, const unsigned char* in, const int bufferLen, unsigned char* out) {
        int result;
        int i;
        int offset = 0;
        int requiredSize = key.getRequiredSize() - 42;

        assert(key.getRequiredSize() * ((dataLen / requiredSize) + (dataLen % requiredSize == 0)) < bufferLen);

        for (i = 0; i + requiredSize < dataLen; i += requiredSize) {
            result = RSA_public_encrypt(requiredSize, in + i, out + offset, key.getRSA(), RSA_PKCS1_OAEP_PADDING);

            if (result < 0) {
                return 0;
            }

            offset += result;
        }

        result = RSA_public_encrypt(dataLen - i, in + i, out + offset, key.getRSA(), RSA_PKCS1_OAEP_PADDING);

        if (result < 0) {
            return 0;
        }

        offset += result;

        return offset;
    }

    int decryptChar(const private_key& key, int dataLen, const unsigned char* in, const int bufferLen, unsigned char* out) {
        int result;
        int offset = 0;
        int requiredSize = key.getRequiredSize();

        assert(dataLen <= bufferLen);

        for (int i = requiredSize; i <= dataLen; i += requiredSize) {
            result = RSA_private_decrypt(requiredSize, in + i - requiredSize, out + offset, key.getRSA(), RSA_PKCS1_OAEP_PADDING);

            if (result < 0) {
                return 0;
            }

            offset += result;
        }

        return offset;
    }

    int signChar(const private_key& key, int dataLen, const unsigned char* in, const int sigLen, unsigned char* signature) {
        int result;
        unsigned char hash[20];

        if (sigLen < key.getRequiredSize()) {
            return -1;
        }

        SHA1(in, dataLen, hash);
        result = RSA_private_encrypt(20, hash, signature, key.getRSA(), RSA_PKCS1_PADDING);

        if (result < 0) {
            return 0;
        }

        return result;
    }

    bool verifyChar(const encryption::public_key& key, int sigLen, const unsigned char* signature, int dataLen, const unsigned char* in) {
        unsigned char hash[20];
        SHA1(in, dataLen, hash);

        unsigned char calcSignatureBuffer[key.getRequiredSize()];

        if (RSA_public_decrypt(sigLen, signature, calcSignatureBuffer, key.getRSA(), RSA_PKCS1_PADDING) == 20) {
            return memcmp(hash, calcSignatureBuffer, 20) == 0;
        }

        return false;
    }

}

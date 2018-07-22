#include "src/encryption/public_key.h"

#include "assert.h"

int encryption::public_key::encryptChar(const int dataLen, const uint8_t* in, const int bufferLen, uint8_t* out) {
    int result;
    int i;
    int offset = 0;
    int requiredSize = required_size() - 42;

    assert(required_size() * ((dataLen / requiredSize) + (dataLen % requiredSize == 0)) < bufferLen);

    for (i = 0; i + requiredSize < dataLen; i += requiredSize) {
        result = RSA_public_encrypt(requiredSize, in + i, out + offset, getRSA(), RSA_PKCS1_OAEP_PADDING);

        if (result < 0) {
            return 0;
        }

        offset += result;
    }

    result = RSA_public_encrypt(dataLen - i, in + i, out + offset, getRSA(), RSA_PKCS1_OAEP_PADDING);

    if (result < 0) {
        return 0;
    }

    offset += result;

    return offset;
    
}


bool encryption::public_key::load_RSA(FILE* file) {
    m_rsa = PEM_read_RSA_PUBKEY(file, nullptr, nullptr, nullptr);
    return m_rsa != nullptr;
}

bool encryption::public_key::save_RSA(BIO* file, EVP_PKEY* pkey) {
    return PEM_write_bio_PUBKEY(file, pkey) == 1;
}

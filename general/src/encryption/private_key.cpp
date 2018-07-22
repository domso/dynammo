#include "src/encryption/private_key.h"

#include "assert.h"

int encryption::private_key::decryptChar(int dataLen, const uint8_t* in, const int bufferLen, uint8_t* out) {
        int result;
        int offset = 0;
        int requiredSize = required_size();

        assert(dataLen <= bufferLen);

        for (int i = requiredSize; i <= dataLen; i += requiredSize) {
            result = RSA_private_decrypt(requiredSize, in + i - requiredSize, out + offset, getRSA(), RSA_PKCS1_OAEP_PADDING);

            if (result < 0) {
                return 0;
            }

            offset += result;
        }

        return offset;
    }

bool encryption::private_key::load_RSA(FILE* file) {
    m_rsa = PEM_read_RSAPrivateKey(file, nullptr, nullptr, nullptr);
    return m_rsa != nullptr;
}

bool encryption::private_key::save_RSA(BIO* file, EVP_PKEY* pkey) {
    return PEM_write_bio_PrivateKey(file, pkey, nullptr, nullptr, 0, nullptr, nullptr) == 1;
}

#include "src/encryption/base_key.h"

encryption::base_key::base_key() {
    m_rsa = nullptr;
    m_requiredSize = 0;
}

encryption::base_key::~base_key() {
    if (m_rsa != nullptr) {
        RSA_free(m_rsa);
    }
}

encryption::base_key::base_key(base_key&& move) : m_rsa(move.m_rsa), m_requiredSize(move.m_requiredSize) {
    move.m_rsa = nullptr;
    move.m_requiredSize = 0;
}

encryption::base_key::base_key(RSA* rsa) : m_rsa(rsa) {
    m_requiredSize = RSA_size(m_rsa);
}

void encryption::base_key::operator=(encryption::base_key && copy) {
    m_rsa = copy.m_rsa;
    m_requiredSize = copy.m_requiredSize;

    copy.m_rsa = nullptr;
    copy.m_requiredSize = 0;
}

RSA* encryption::base_key::getRSA() const {
    return m_rsa;
}

bool encryption::base_key::valid() const {
    return m_rsa != nullptr;
}

bool encryption::base_key::load(const std::string filename) {
    FILE* file = fopen(filename.c_str(), "rb");

    if (file != nullptr) {
        bool result = load_RSA(file);
        fclose(file);
        m_requiredSize = RSA_size(m_rsa);
        return result;
    }

    return false;
}

bool encryption::base_key::save(const std::string filename) {
    bool result = true;
    EVP_PKEY* pkey = EVP_PKEY_new();
    BIO* file = BIO_new_file(filename.c_str(), "w");

    if (EVP_PKEY_set1_RSA(pkey, m_rsa) == 1 && file != nullptr) {
        result = save_RSA(file, pkey);
    }

    EVP_PKEY_free(pkey);
    BIO_free(file);

    return result;
}

int encryption::base_key::required_size() const {
    return m_requiredSize;
}


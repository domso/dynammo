#include "src/encryption/keygen.h"
#include "src/encryption/randomize.h"

std::optional<std::pair<encryption::public_key, encryption::private_key>> encryption::keygen::generate() {
    RSA* keys = RSA_new();
    BIGNUM* exponent = BN_new();
    
    if (BN_set_word(exponent, RSA_F4) != 1) {
        RSA_free(keys);
        BN_free(exponent);
        return std::nullopt;
    }
    
    if (RSA_generate_key_ex(keys, 2048, exponent, nullptr) != 1) {
        RSA_free(keys);
        BN_free(exponent);
        return std::nullopt;
    }
    
    char seed[256];
    randomize(seed);   
    RAND_seed(&seed, 256);

    RSA* publicKey = RSAPublicKey_dup(keys);
    RSA* privateKey = RSAPrivateKey_dup(keys);
    RSA_free(keys);
    BN_free(exponent);          
    
    return std::pair<encryption::public_key, encryption::private_key>(publicKey, privateKey);
}


#ifndef general_encryption_randomize_h
#define general_encryption_randomize_h

#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>

namespace encryption {
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Description:
    // - fills generic object of type T with random bytes
    // Parameter:
    // - in: reference to object
    // Return:
    // - true  | on success
    // - false | on any error
    template <typename T>
    bool randomize(T& in) {
        return RAND_bytes((unsigned char*)&in, sizeof(T)) == 1;
    }
}

#endif

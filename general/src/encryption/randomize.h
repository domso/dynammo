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
    //______________________________________________________________________________________________________
    //
    // Description:
    // - fills generic objects of type T with random bytes
    // Parameter:
    // - in: reference to object
    // - n: number of objects
    // Return:
    // - true  | on success
    // - false | on any error
    //______________________________________________________________________________________________________
    template <typename T>
    bool randomize(T* in, const int n = 1) {
        return RAND_bytes((unsigned char*)in, sizeof(T) * n) == 1;
    }
}

#endif

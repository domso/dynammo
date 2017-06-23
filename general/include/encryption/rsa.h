#ifndef general_encryption_rsa_h
#define general_encryption_rsa_h

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
    // abstract general key
    //______________________________________________________________________________________________________
    class base_key {
    public:
        base_key();
        ~base_key();
        //______________________________________________________________________________________________________
        //
        // Return: pointer to the internal RSA-instance
        //______________________________________________________________________________________________________
        RSA* getRSA() const;
        //______________________________________________________________________________________________________
        //
        // Description:
        // - tries to load a rsa-key specified by the filename
        // - the actual implementation (private or public key) depends on the inherited class
        // Parameter:
        // - filename: the absolut or relative path to a valid rsa-key in .pem format
        //______________________________________________________________________________________________________
        bool load(const std::string filename);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - the output size of rsa-encryption depends on the size of the key and the padding algorithms
        // - see encryptChar(), decryptChar(), signChar() and verifyChar() for more details!
        // Return:
        // - the required size for encryption(output) and decryption(input)
        //______________________________________________________________________________________________________
        int getRequiredSize() const;
    protected:
        //______________________________________________________________________________________________________
        //
        // Description:
        // - an inherited class needs to implement a methode to load a key
        // - this is used by load()
        // Parameter:
        // - file: opened and valid file-descriptor to the key
        // Return:
        // - true  | on success
        // - false | on any error
        //______________________________________________________________________________________________________

        virtual bool setRSA(FILE* file) = 0;
        // pointer to internal RSA-structure
        RSA* rsa_;
        // store the result of getRequiredSize()
        int requiredSize_;

    };
    //______________________________________________________________________________________________________
    //
    // private rsa-key
    //______________________________________________________________________________________________________
    class private_key : public base_key {
        //______________________________________________________________________________________________________
        //
        // Description:
        // - an inherited class needs to implement a methode to load a key
        // - this is used by load()
        // Parameter:
        // - file: opened and valid file-descriptor to the private-key
        // Return:
        // - true  | on success
        // - false | on any error
        //______________________________________________________________________________________________________
        bool setRSA(FILE* file);
    };
    //______________________________________________________________________________________________________
    //
    // public rsa-key
    //______________________________________________________________________________________________________
    class public_key : public base_key {
        //______________________________________________________________________________________________________
        //
        // Description:
        // - an inherited class needs to implement a methode to load a key
        // - this is used by load()
        // Parameter:
        // - file: opened and valid file-descriptor to the public-key
        // Return:
        // - true  | on success
        // - false | on any error
        //______________________________________________________________________________________________________
        bool setRSA(FILE* file);
    };
    //______________________________________________________________________________________________________
    //
    // Description:
    // - tries to encrypt dataLen bytes from 'in' with the public key and store the encrypted bytes into 'out'
    // - for encryption RSA_public_encrypt() with RSA_PKCS1_OAEP_PADDING is used
    // - the required size for the output-buffer will be checked
    // - the input will be splitted into "key.getRequiredSize"-42 input-blocks
    // - each input-block will be encrypted to a "key.getRequiredSize()" large output block
    // Parameters:
    // - key: reference to a valid rsa-public-key
    // - dataLen: length of the input buffer
    // - in: pointer to the input buffer
    // - bufferLen: length of the output buffer
    // - out: pointer to the output buffer
    // Return:
    // - number of valid encrypted bytes in the output buffer | on success
    // - zero | on any error
    //______________________________________________________________________________________________________
    int encryptChar(const encryption::public_key& key, const int dataLen, const unsigned char* in, const int bufferLen, unsigned char* out);
    //______________________________________________________________________________________________________
    //
    // Description:
    // - tries to decrypt dataLen bytes from 'in' with the private key and store the decrypted bytes into 'out'
    // - for decryption RSA_private_decrypt() with RSA_PKCS1_OAEP_PADDING is used
    // - the required size for the output-buffer will be checked
    // - the input will be splitted into "key.getRequiredSize" input-blocks
    // - each input-block will be decrypted to a "key.getRequiredSize()" large output block
    // Parameters:
    // - key: reference to a valid rsa-private-key
    // - dataLen: length of the input buffer
    // - in: pointer to the input buffer
    // - bufferLen: length of the output buffer
    // - out: pointer to the output buffer
    // Return:
    // - number of valid decrypted bytes in the output buffer | on success
    // - zero | on any error
    //______________________________________________________________________________________________________
    int decryptChar(const private_key& key, int dataLen, const unsigned char* in, const int bufferLen, unsigned char* out);
    //______________________________________________________________________________________________________
    //
    // Description:
    // - tries to sign dataLen bytes from 'in' with the private key and store the signature into 'out'
    // - for signing RSA_private_encrypt() with SHA1 and RSA_PKCS1_PADDING is used
    // - the required size for the output-buffer will be checked
    // Parameters:
    // - key: reference to a valid rsa-public-key
    // - dataLen: length of the input buffer
    // - in: pointer to the input buffer
    // - bufferLen: length of the output buffer
    // - out: pointer to the output buffer
    // Return:
    // - number of valid signature bytes in the output buffer | on success
    // - zero | on any error
    //______________________________________________________________________________________________________
    int signChar(const encryption::private_key& key, int dataLen, const unsigned char* in, const int bufferLen, unsigned char* out);
    //______________________________________________________________________________________________________
    //
    // Description:
    // - tries to validate dataLen bytes from 'in' with the public key
    // - for validation RSA_public_decrypt() with SHA1 and RSA_PKCS1_PADDING is used
    // - the required size for the output-buffer will be checked
    // Parameters:
    // - key: reference to a valid rsa-public-key
    // - sigLen: length of signature
    // - signature: pointer to the signature buffer
    // - dataLen: length of the input buffer
    // - in: pointer to the input buffer
    // Return:
    // - true  | on success
    // - false | on any error
    //______________________________________________________________________________________________________
    bool verifyChar(const encryption::public_key& key, int sigLen, const unsigned char* signature, int dataLen, const unsigned char* in);
    //______________________________________________________________________________________________________
    //
    // simple struct with template-defined size
    //______________________________________________________________________________________________________
    template<int T>
    struct encryptedData_RSA {
        char data[T * 8];
    };
}

#endif

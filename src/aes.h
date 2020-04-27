/* 
    This is a C++ implementation of the AES (Advanced Encryption Standard), based on the Rinjdael block cipher algorithm.
    Official specification can be found here: https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf
*/

#pragma once

#include <stdint.h>

#define AES_DEBUG 1

#ifdef AES_DEBUG
static void display_word (uint8_t *word, const char *debug);
#endif

// number of columns in one block.
#define Nb 4
// number of lines in one block (not actually part of the specification).
#define Nl 4

// Possible key sizes for the AES constructor.
enum AESKeySize {
    AES_128 = 128,
    AES_192 = 192,
    AES_256 = 256
};

class AES {
    
    private:

    // the chosen key size/
    AESKeySize keySize;

    /* 
        Nk represents the number of words (4 bytes) in the input encryption key.

        Will take the following values when constructing the AES instance:

        4 in 128 bits key mode.
        6 in 192 bits key mode.
        8 in 256 bits key mode.

    */

    uint8_t Nk; 

    /* 
        Nr represents the numbers of rounds needed for each key size in the AES cipher.

        Will take the following values constructing the AES instance:

        10 in 128 bits key mode.
        12 in 192 bits key mode.
        14 in 256 bits key mode.

    */

    uint8_t Nr; 

    /* The buffer that holds the generated round keys from the key expansion step. */

    uint8_t *RoundKeys;

    // Key expansion step, in order to produces keys for each round from the main one.
    void ExpandKey (uint8_t *key);

    // These two methods are used within the key expansion step.
    void RotWord(uint8_t *word);
    void SubWord(uint8_t *word);
    
    // Apply S-box substitution to our state matrix.
    void SubBytes(uint8_t state[Nb][Nl]);
    void InvSubBytes(uint8_t state[Nb][Nl]);

    // Apply rows shifting to our state matrix.
    void ShiftRows(uint8_t state[Nb][Nl]);
    void InvShiftRows(uint8_t state[Nb][Nl]);

    // Apply columns mixing to our state matrix.
    void MixColumns(uint8_t state[Nb][Nl]);
    void InvMixColumns(uint8_t state[Nb][Nl]);

    // XOR the state with the round key
    void AddRoundKey(uint8_t state[Nb][Nl], uint8_t roundNumber);

    // This method is used to multiply a number by X in the galois field G(2^8) (so in this field, X value is 2)
    uint8_t XTime(uint8_t number);

    // This method allows us to multiply any byte by another number (up to 15) in the galois field G(2^8)
    uint8_t GMultiply (uint8_t number, uint8_t multiplier); 
    
    public:

    // The main block encryption method.
    void Cipher(uint8_t state[Nb][Nl]);

    // The AES constructor.
    // First parameter is the key.
    // Second parameter is the key size.
    AES(uint8_t *key, AESKeySize keySize);
    ~AES();

    /* 
        Encrypts a blob of data using AES in Cipher Block Chaining (CBC) mode of operation.
        
        The "buffer" parameter is a pointer to the buffer which is to be encrypted. 
        Its content will be overwritten with the resulting encrypted buffer.
        
        PKCS#7 padding is REQUIRED. Which means you need to pad your buffer with N bytes with the value of N, where N is the length of the buffer in bytes modulo the AES cipher block size (16).
        
        Check these examples for better understanding:

        If you have a 13 byte buffer (which is NOT a multiple of th AES block size, i.e 16 bytes), you need to pad with 3 more bytes:

        00 01 02 03 04 05 06 07 08 09 0A 0B 0C = 13 bytes
        -------------- original data ---------

        becomes...

        00   01   02   03   04   05   06   07   08   09   0A   0B   0C   03     03    03 = 16 bytes
        ------------------------- original data ----------------------   --- padding ---
        
        If you have a 16 bytes buffer (which is EXACTLY a multiple of the AES block size, i.e 16 bytes), you need to pad with a whole new block:
        
        00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0F 10 = 16 bytes
        -------------- original data ------------------

        becomes...

        00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0F 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10  = 32 bytes
        -------------- original data ------------------ ----------------- padding ---------------------

        
        The "length" parameter is the size in bytes of the data within "buffer" to be encrypted. 
        Of course, according to the buffer "parameter" constraints, it must be a multiple of the block size in AES (128 bits).

        The "key" parameter is the encryption key to use. It must be the same size as the request key type when the AES instance was created (128, 192 or 256).

        The "iv" parameter is the initialization vector for the CBC mode, it must be an unpredictable value but is not to be kept secret after the encryption process.
        It must have the same size as the AES block size, which is 16 bytes (128 bits).

    */

    void EncryptCBC(uint8_t *buffer, uint8_t length, uint8_t *key, uint8_t *iv);

    /* 
        Decrypts a blob of data using AES in Cipher Block Chaining (CBC) mode of operation.

        The "buffer" parameter is a pointer to the buffer which is to be decrypted. 
        Its content will be overwritten with the resulting decrypted buffer (padding won't be stripped).

        The "length" parameter is the length of "buffer", in bytes. 
        It needs to be a multiple of the AES cipher block size (16 bytes).

        The "key" parameter is a the key that was used for encryption. It must be the same size as the request key type when the AES instance was created (128, 192 or 256).

        The "iv" parameter is the initialization vector that was used for the CBC mode.
        It must have the same size as the AES block size, which is 16 bytes (128 bits).

    */

    void DecryptCBC(uint8_t *buffer, uint8_t length, uint8_t *key, uint8_t *iv);
   
    // These two methods must not be used. ECB mode was implemented for testing purposes since it doesn't involve any changes to the Rijndael algorithm's output for each block (it simply concatenate the ciphertext blocks).
    void EncryptECB(uint8_t *buffer, uint8_t length, uint8_t *key);
    void DecryptECB(uint8_t *buffer, uint8_t length, uint8_t *key);
};
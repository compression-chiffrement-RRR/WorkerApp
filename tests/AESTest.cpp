#include <gtest/gtest.h>
#include <stdint.h>
#include <cstring>
#include "../src/aes.h"
#include "../src/debug.h"

class AESTest : public ::testing::Test {};

TEST(AES128, EncryptAndDecryptECB){

    uint8_t input [] = {
        0x32, 0x43, 0xf6, 0xa8, 
        0x88, 0x5a, 0x30, 0x8d, 
        0x31, 0x31, 0x98, 0xa2, 
        0xe0, 0x37, 0x07, 0x34
    };

    uint8_t key [] = {
        0x2b, 0x7e, 0x15, 0x16, 
        0x28, 0xae, 0xd2, 0xa6, 
        0xab, 0xf7, 0x15, 0x88, 
        0x09, 0xcf, 0x4f, 0x3c
    };

    uint8_t expectedCiphertext [] = {
        0X39, 0x25, 0x84, 0x1d,
        0x02, 0xdc, 0x09, 0xfb,
        0xdc, 0x11, 0x85, 0x97,
        0x19, 0x6a, 0x0b, 0x32
    };

    uint8_t *expectedPlaintext = new uint8_t[sizeof(input)];
    
    memcpy(expectedPlaintext, input, sizeof(input));

    AES aes = AES(AESKeySize::AES_128, AESMode::ECB, key);

    aes.Encrypt(input, sizeof(input));

    ASSERT_EQ(0, memcmp(input, expectedCiphertext, sizeof(input)));

    aes.Decrypt(input, sizeof(input));

    ASSERT_EQ(0, memcmp(input, expectedPlaintext, sizeof(input)));

    delete [] expectedPlaintext;

}

TEST(AES192, EncryptAndDecryptECB){

    uint8_t input [] = {
        0x00, 0x11, 0x22, 0x33, 
        0x44, 0x55, 0x66, 0x77, 
        0x88, 0x99, 0xaa, 0xbb, 
        0xcc, 0xdd, 0xee, 0xff
    };

    uint8_t key [] = {
        0x00, 0x01, 0x02, 0x03, 
        0x04, 0x05, 0x06, 0x07, 
        0x08, 0x09, 0x0a, 0x0b, 
        0x0c, 0x0d, 0x0e, 0x0f, 
        0x10, 0x11, 0x12, 0x13, 
        0x14, 0x15, 0x16, 0x17
    };

    uint8_t expectedCiphertext [] = {
        0xdd, 0xa9, 0x7c, 0xa4, 
        0x86, 0x4c, 0xdf, 0xe0, 
        0x6e, 0xaf, 0x70, 0xa0, 
        0xec, 0x0d, 0x71, 0x91
    };

    uint8_t *expectedPlaintext = new uint8_t[sizeof(input)];
    
    memcpy(expectedPlaintext, input, sizeof(input));

    AES aes = AES(AESKeySize::AES_192, AESMode::ECB, key);

    aes.Encrypt(input, sizeof(input));

    ASSERT_EQ(0, memcmp(input, expectedCiphertext, sizeof(input)));

    aes.Decrypt(input, sizeof(input));

    ASSERT_EQ(0, memcmp(input, expectedPlaintext, sizeof(input)));

    delete [] expectedPlaintext;

}

TEST(AES256, EncryptAndDecryptECB){

    uint8_t input [] = {
        0x00, 0x11, 0x22, 0x33, 
        0x44, 0x55, 0x66, 0x77, 
        0x88, 0x99, 0xaa, 0xbb, 
        0xcc, 0xdd, 0xee, 0xff
    };

    uint8_t key [] = {
        0x00, 0x01, 0x02, 0x03, 
        0x04, 0x05, 0x06, 0x07, 
        0x08, 0x09, 0x0a, 0x0b, 
        0x0c, 0x0d, 0x0e, 0x0f, 
        0x10, 0x11, 0x12, 0x13, 
        0x14, 0x15, 0x16, 0x17, 
        0x18, 0x19, 0x1a, 0x1b, 
        0x1c, 0x1d, 0x1e, 0x1f
    };

    uint8_t expectedCiphertext [] = {
        0x8e, 0xa2, 0xb7, 0xca, 
        0x51, 0x67, 0x45, 0xbf, 
        0xea, 0xfc, 0x49, 0x90, 
        0x4b, 0x49, 0x60, 0x89
    };

    uint8_t *expectedPlaintext = new uint8_t[sizeof(input)];
    
    memcpy(expectedPlaintext, input, sizeof(input));

    AES aes = AES(AESKeySize::AES_256, AESMode::ECB, key);

    aes.Encrypt(input, sizeof(input));

    ASSERT_EQ(0, memcmp(input, expectedCiphertext, sizeof(input)));

    aes.Decrypt(input, sizeof(input));

    ASSERT_EQ(0, memcmp(input, expectedPlaintext, sizeof(input)));

    delete [] expectedPlaintext;

}

TEST(AES128, EncryptAndDecryptCBC){

    uint8_t input [] = {
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 
        0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 
        0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf
    };

    uint8_t key [] = { 
        0x56, 0xe4, 0x7a, 0x38, 
        0xc5, 0x59, 0x89, 0x74, 
        0xbc, 0x46, 0x90, 0x3d, 
        0xba, 0x29, 0x03, 0x49
    };

    uint8_t iv [] = {
        0x8c, 0xe8, 0x2e, 0xef, 
        0xbe, 0xa0, 0xda, 0x3c, 
        0x44, 0x69, 0x9e, 0xd7, 
        0xdb, 0x51, 0xb7, 0xd9
    };

    uint8_t expectedCiphertext [] = {
        0xc3, 0x0e, 0x32, 0xff, 0xed, 0xc0, 0x77, 0x4e, 0x6a, 0xff, 0x6a, 0xf0, 0x86, 0x9f, 0x71, 0xaa, 
        0x0f, 0x3a, 0xf0, 0x7a, 0x9a, 0x31, 0xa9, 0xc6, 0x84, 0xdb, 0x20, 0x7e, 0xb0, 0xef, 0x8e, 0x4e, 
        0x35, 0x90, 0x7a, 0xa6, 0x32, 0xc3, 0xff, 0xdf, 0x86, 0x8b, 0xb7, 0xb2, 0x9d, 0x3d, 0x46, 0xad, 
        0x83, 0xce, 0x9f, 0x9a, 0x10, 0x2e, 0xe9, 0x9d, 0x49, 0xa5, 0x3e, 0x87, 0xf4, 0xc3, 0xda, 0x55
    };

    uint8_t *expectedPlaintext = new uint8_t[sizeof(input)];
    
    memcpy(expectedPlaintext, input, sizeof(input));

    AES aes = AES(AESKeySize::AES_128, AESMode::CBC, key, iv);

    aes.Encrypt(input, sizeof(input));

    ASSERT_EQ(0, memcmp(input, expectedCiphertext, sizeof(input)));

    aes.SetIv(iv);
    aes.Decrypt(input, sizeof(input));
    
    ASSERT_EQ(0, memcmp(input, expectedPlaintext, sizeof(input)));

    delete [] expectedPlaintext;
}
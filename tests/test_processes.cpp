#include <gtest/gtest.h>
#include <string>
#include <stdlib.h>

#include "../src/Huffman.h"
#include "../src/AES.h"

#include "utils.h"

using namespace std;

TEST(HuffmanTest, EncryptAndCompress_File){

    Huffman huffman;
    
    uint8_t key [16] = { 0, 1, 2, 3, 4, 5, 6, 7, (uint8_t)11111, 9, 10, 11, 12, 13, 14, 15 };
    
    AES aes(AESKeySize::AES_128, AESMode::ECB, key);

    const string original = "/root/tests/testfiles/iso.txt";
    const string encrypted = "/root/tests/testfiles/iso.encrypted.txt";
    const string compressed = "/root/tests/testfiles/iso.compressed.txt";
    const string decompressed = "/root/tests/testfiles/iso.decompressed.txt";
    const string decrypted = "/root/tests/testfiles/iso.decrypted.txt";

    ASSERT_EQ(0, aes.EncryptFile(original, encrypted));
    ASSERT_EQ(true, huffman.CompressFile(encrypted, compressed));
    ASSERT_EQ(true, huffman.DecompressFile(compressed, decompressed));
    ASSERT_EQ(0, aes.DecryptFile(decompressed, decrypted));

    ASSERT_EQ(true, compareFiles(original, decrypted));

    ASSERT_EQ(0, remove(encrypted.c_str()));
    ASSERT_EQ(0, remove(compressed.c_str()));
    ASSERT_EQ(0, remove(decompressed.c_str()));
    ASSERT_EQ(0, remove(decrypted.c_str()));
};
#include <gtest/gtest.h>
#include <string>
#include <stdlib.h>

using namespace std;

#include "../src/Huffman.h"

TEST(HuffmanTest, CompressAndDecompress_Huffman_File){

    Huffman h;
    
    const string original = "/root/tests/testfiles/lorem.txt";
    const string compressed = "/root/tests/testfiles/lorem.txt.compressed";
    const string decompressed = "/root/tests/testfiles/lorem.txt.decompressed";

    ASSERT_EQ(true, h.CompressFile(original, compressed));
    ASSERT_EQ(true, h.DecompressFile(compressed, decompressed));

    ASSERT_EQ(0, remove(compressed.c_str()));
    ASSERT_EQ(0, remove(decompressed.c_str()));
};

TEST(HuffmanTest, CompressAndDecompress_Huffman_File_OneChar){

    Huffman h;
    
    const string original = "/root/tests/testfiles/onechar.txt";
    const string compressed = "/root/tests/testfiles/onechar.txt.compressed";
    const string decompressed = "/root/tests/testfiles/onechar.txt.decompressed";

    ASSERT_EQ(true, h.CompressFile(original, compressed));
    ASSERT_EQ(true, h.DecompressFile(compressed, decompressed));

    ASSERT_EQ(0, remove(compressed.c_str()));
    ASSERT_EQ(0, remove(decompressed.c_str()));
};
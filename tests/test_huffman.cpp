#include <gtest/gtest.h>
#include <string>
#include <stdlib.h>

using namespace std;

#include "../src/Huffman.h"

#include "utils.h"

TEST(HuffmanTest, CompressAndDecompress_Huffman_File){

    Huffman h;
    
    const string original = "/app/tests/testfiles/lorem.txt";
    const string compressed = "/app/tests/testfiles/lorem.txt.compressed";
    const string decompressed = "/app/tests/testfiles/lorem.txt.decompressed";

    ASSERT_EQ(true, h.CompressFile(original, compressed));
    ASSERT_EQ(true, h.DecompressFile(compressed, decompressed));

    ASSERT_EQ(true, compareFiles(original, decompressed));

    ASSERT_EQ(0, remove(compressed.c_str()));
    ASSERT_EQ(0, remove(decompressed.c_str()));
};

TEST(HuffmanTest, CompressAndDecompress_Huffman_File_OneChar){

    Huffman h;
    
    const string original = "/app/tests/testfiles/onechar.txt";
    const string compressed = "/app/tests/testfiles/onechar.txt.compressed";
    const string decompressed = "/app/tests/testfiles/onechar.txt.decompressed";

    ASSERT_EQ(true, h.CompressFile(original, compressed));
    ASSERT_EQ(true, h.DecompressFile(compressed, decompressed));

    ASSERT_EQ(true, compareFiles(original, decompressed));

    ASSERT_EQ(0, remove(compressed.c_str()));
    ASSERT_EQ(0, remove(decompressed.c_str()));
};
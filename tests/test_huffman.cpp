#include <gtest/gtest.h>
#include <string>

using namespace std;

#include "../src/Huffman.h"

TEST(HuffmanTest, CompressAndDecompress_Huffman_File){

    Huffman h;
    
    const string original = "/root/tests/testfiles/onechar.txt";
    const string compressed = "/root/tests/testfiles/onechar.txt.compressed";
    const string decompressed = "/root/tests/testfiles/onechar.txt.decompressed";

    h.CompressFile(original, compressed);
    h.DecompressFile(compressed, decompressed);
};
#include <gtest/gtest.h>
#include <string>

#include "../src/HttpClient.h"

using namespace std;

TEST(HttpClientTest, DownloadFile){

    HttpClient client;
    const string url = "https://the.earth.li/~sgtatham/putty/latest/w32/putty.exe";
    const string output = "/root/tests/testfiles/putty.exe";

    ASSERT_EQ(0, client.DownloadFile(url, output));

    remove(output.c_str());

}
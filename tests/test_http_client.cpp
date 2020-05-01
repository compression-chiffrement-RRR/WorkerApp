#include <gtest/gtest.h>

#include "../src/http_client.h"


TEST(HttpClientTest, DownloadFile){

    HttpClient client;
    const char *url = "https://the.earth.li/~sgtatham/putty/latest/w32/putty.exe";
    const char *output = "/root/tests/testfiles/putty.exe";

    ASSERT_EQ(0, client.DownloadFile(url, output));

    remove(output);

}
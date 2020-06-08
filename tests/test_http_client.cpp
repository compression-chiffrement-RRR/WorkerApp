#include <gtest/gtest.h>
#include <string>
#include <nlohmann/json.hpp>

#include "../src/HttpClient.h"

using namespace std;
using json = nlohmann::json;

TEST(HttpClientTest, DownloadFile){

    HttpClient client;
    const string url = "https://the.earth.li/~sgtatham/putty/latest/w32/putty.exe";
    const string output = "/root/tests/testfiles/putty.exe";

    ASSERT_EQ(true, client.DownloadFile(url, output));

    ASSERT_EQ(0, remove(output.c_str()));

}

TEST(HttpClientTest, SendPostRequest){

    HttpClient client;
    json body = {
        { "hello", "world" },
    };

    //ASSERT_EQ(true, client.SendPostRequest("https://webhook.site", body));

}
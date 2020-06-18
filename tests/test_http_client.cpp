#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>

#include <curl/curl.h>

#include "../src/HttpClient.h"

using namespace std;
using json = nlohmann::json;

static size_t ReadJsonCallback(void *data, size_t size, size_t nmemb, void *userPtr){
    string *str = (string *)userPtr;
    for (size_t i = 0; i < size * nmemb; i ++){
        (*str).push_back(((char *)data)[i]);
    }
    return size * nmemb;
};

namespace {

    class HttpTestSuite : public testing::Test {

        protected:

        string webhookToken;
        
        void SetUp () override {
            
            CURL *curl = curl_easy_init();
            struct curl_slist *headers = NULL;

            string response;

            json payload = {
                {" default_status", 200 },
                { "default_content", "" },
                { "default_content_type", "text/html" },
                { "timeout", 0 },
                { "cors", false} ,
                { "expiry", true }
            };

            string jsonString = payload.dump();

            curl_easy_setopt(curl, CURLOPT_URL, "https://webhook.site/token");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ReadJsonCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &(response));
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.data());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, jsonString.size());

            headers = curl_slist_append(headers, "Content-Type: application/json");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            curl_easy_perform(curl);

            json jsonResponse = json::parse(response);

            this->webhookToken = jsonResponse["uuid"];

            curl_easy_cleanup(curl);

        }
    };

    TEST_F(HttpTestSuite, HttpTests){
        
        HttpClient client;
        const string url = "https://the.earth.li/~sgtatham/putty/latest/w32/putty.exe";
        const string output = "/root/tests/testfiles/putty.exe";

        ASSERT_EQ(true, client.DownloadFile(url, output));

        ASSERT_EQ(0, remove(output.c_str()));

        const string webhook = string("https://webhook.site/") + this->webhookToken;
    
        json body = {
            { "hello", "world" },
        };

        ASSERT_EQ(true, client.SendPostRequest(webhook, body));

        const string filenamePath = "/root/tests/testfiles/lorem.txt";

        ASSERT_EQ(true, client.UploadFile(webhook, filenamePath));
    }

}
#pragma once

#include <nlohmann/json.hpp>
#include <curl/curl.h>

using json = nlohmann::json;

class HttpClient {

    private:

    CURL *curl = NULL;
    
    public:

    HttpClient ();
    ~HttpClient ();
    
    bool DownloadFile (const std::string& url, const std::string& outputFile);
    bool SendPostRequest(const std::string& url, const json& body);
};
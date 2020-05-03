#pragma once

#include <curl/curl.h>

class HttpClient {

    private:

    CURL *curl = NULL;
    
    public:

    HttpClient ();
    ~HttpClient ();
    
    int DownloadFile (const std::string& url, const std::string& outputFile);
};
#include <curl/curl.h>
#include <fstream>
#include <iostream>

#include "HttpClient.h"

#include "debug.h"

using namespace std;

static size_t DownloadFile_WriteDataCallback(void *data, size_t size, size_t nmemb, void *userPtr);

HttpClient::HttpClient (){
    
    this->curl = curl_easy_init();
};

HttpClient::~HttpClient (){
    
    if (this->curl != NULL)
        curl_easy_cleanup(this->curl);
};

static size_t DownloadFile_WriteDataCallback(void *data, size_t size, size_t nmemb, void *userPtr){

    ofstream *outFile = (ofstream *)userPtr;

    if (outFile == nullptr || !(*outFile).is_open())
        return 0;

    (*outFile).write((char *)data, size * nmemb);

    if ((*outFile).bad() || (*outFile).fail())
        return 0;

    return size * nmemb;
}

bool HttpClient::DownloadFile(const string& url, const string& outputFile){

    bool ret = false;
    ofstream outFile;

    if (this->curl == NULL)
        goto clean;

    outFile.open(outputFile, ofstream::binary|ofstream::trunc);

    if (!outFile.is_open())
        goto clean;

    curl_easy_setopt(this->curl, CURLOPT_URL, url.c_str());

    curl_easy_setopt(this->curl, CURLOPT_FOLLOWLOCATION, 1L);
    
    curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, DownloadFile_WriteDataCallback);
    curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, &outFile); 

    if (curl_easy_perform(this->curl) != CURLE_OK)
        goto clean;

    ret = true;

clean:

    if (this->curl != NULL)
        curl_easy_reset(this->curl);

    if (outFile.is_open())
        outFile.close();

    return ret;
};

bool HttpClient::SendPostRequest(const std::string& url, const json& body){

    bool ret = false;

    struct curl_slist *headers = NULL;
    long code = -1L;

    if (this->curl == NULL)
        goto clean;

    headers = curl_slist_append(headers, "Content-Type: application/json");

    if (headers == NULL)
        goto clean;
    
    curl_easy_setopt(this->curl, CURLOPT_URL, url.c_str());

    curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(this->curl, CURLOPT_POST, 1L);
    curl_easy_setopt(this->curl, CURLOPT_COPYPOSTFIELDS, body.dump().c_str()); 

    if(curl_easy_perform(this->curl) != CURLE_OK)
        goto clean;

    if (curl_easy_getinfo(this->curl, CURLINFO_RESPONSE_CODE, &code) != CURLE_OK || code < 200 || code >= 300)
        goto clean;

    ret = true;

clean:

    if (headers != NULL)
        curl_slist_free_all(headers);

    if (this->curl != NULL)
        curl_easy_reset(this->curl);

    return ret;
 
};

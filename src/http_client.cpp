#include <curl/curl.h>
#include <fstream>

#include "http_client.h"

#include "debug.h"

using namespace std;

static size_t DownloadFile_WriteDataCallback(void *data, size_t size, size_t nmemb, void *userPtr);

HttpClient::HttpClient (){
    
    this->curl = curl_easy_init();
};

HttpClient::~HttpClient (){
    
    if (this->curl != nullptr)
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

int HttpClient::DownloadFile(const char *url, const char *outputFile){

    int ret = -1;
    ofstream outFile;

    if (this->curl == nullptr)
        goto clean;

    outFile.open(outputFile, ofstream::binary|ofstream::trunc);

    if (!outFile.is_open())
        goto clean;

    curl_easy_setopt(this->curl, CURLOPT_URL, url);

    curl_easy_setopt(this->curl, CURLOPT_FOLLOWLOCATION, 1L);
    
    curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, DownloadFile_WriteDataCallback);
    curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, &outFile);

    if (curl_easy_perform(this->curl) != CURLE_OK)
        goto clean;

    ret = 0;

clean:

    if (this->curl != nullptr)
        curl_easy_reset(this->curl);

    if (outFile.is_open())
        outFile.close();

    return ret;
};

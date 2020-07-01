#include <curl/curl.h>
#include <fstream>
#include <iostream>

#include "HttpClient.h"

#include "debug.h"

using namespace std;

 size_t DownloadFile_WriteDataCallback(void *data, size_t size, size_t nmemb, void *userPtr);
 size_t UploadFile_ReadDataCallback(void *ptr, size_t size, size_t nmemb, void *userPtr);

HttpClient::HttpClient (){

    this->curl = curl_easy_init();

    if (this->curl != NULL){
        curl_easy_setopt (this->curl, CURLOPT_VERBOSE, 0L);
    }
};

HttpClient::~HttpClient (){
    
    if (this->curl != NULL)
        curl_easy_cleanup(this->curl);
};

 size_t DownloadFile_WriteDataCallback(void *data, size_t size, size_t nmemb, void *userPtr){

    ofstream *outFile = (ofstream *)userPtr;

    if (outFile == nullptr || !(*outFile).is_open()){
        cout << "WriteDataCallback IO error." << endl;
        return 0;
    }

    (*outFile).write((char *)data, size * nmemb);

    if ((*outFile).bad() || (*outFile).fail()){
        cout << "WriteDataCallback IO error." << endl;
        return 0;
    }

    return size * nmemb;
}

 size_t UploadFile_ReadDataCallback(void *data, size_t size, size_t nmemb, void *userPtr){

    ifstream *inFile = (ifstream *)userPtr;

    if (inFile == nullptr || !(*inFile).is_open()){
        cout << "ReadDataCallback IO error." << endl;
        return CURL_READFUNC_ABORT;
    }

    (*inFile).read((char *)data, size * nmemb);

    if ((*inFile).bad() || ((*inFile).fail() && !(*inFile).eof())){
        cout << "ReadDataCallback IO error." << endl;
        return CURL_READFUNC_ABORT;
    }

    return (*inFile).gcount();
}

bool HttpClient::DownloadFile(const string& url, const string& outputFile){

    bool ret = false;
    ofstream outFile;
    CURLcode curlCode;

    if (this->curl == NULL)
        goto clean;

    outFile.open(outputFile, ofstream::binary|ofstream::trunc);

    if (!outFile.is_open()){
        cout << "Couldn't open output file " << outputFile << " for download." <<endl;
        goto clean;
    }

    curl_easy_setopt(this->curl, CURLOPT_URL, (char *)url.c_str());

    curl_easy_setopt(this->curl, CURLOPT_FOLLOWLOCATION, 1L);
    
    curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, DownloadFile_WriteDataCallback);
    curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, &outFile); 

    curlCode = curl_easy_perform(this->curl);

    if (curlCode != CURLE_OK){
        cout << "Error while downloading " << url << " (" << curl_easy_strerror(curlCode) << ")" << endl;
        goto clean;
    }

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
    CURLcode curlCode;

    if (this->curl == NULL)
        goto clean;

    headers = curl_slist_append(headers, "Content-Type: application/json");

    if (headers == NULL){
        cout << "Failed to add headers during POST request." << endl;
        goto clean;
    }
    
    curl_easy_setopt(this->curl, CURLOPT_URL, (char *)url.c_str());

    curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(this->curl, CURLOPT_POST, 1L);
    curl_easy_setopt(this->curl, CURLOPT_COPYPOSTFIELDS, body.dump().c_str()); 

    curlCode = curl_easy_perform(this->curl);

    if (curlCode != CURLE_OK){
        cout << "Error while performing POST request (" << curl_easy_strerror(curlCode) << ")" << endl;
        goto clean;
    }

    if (curl_easy_getinfo(this->curl, CURLINFO_RESPONSE_CODE, &code) != CURLE_OK || code < 200 || code >= 300){
        cout << "Bad response code after sending POST request (" << code << ")" << endl;
        goto clean;
    }

    ret = true;

clean:

    if (headers != NULL)
        curl_slist_free_all(headers);

    if (this->curl != NULL)
        curl_easy_reset(this->curl);

    return ret;
 
};

bool HttpClient::UploadFile(const std::string& url, const std::string& fileName){

    bool ret = false;
    ifstream inFile;
    streampos fileSize = 0;
    long code = -1L;
    struct curl_slist *headers = NULL;
    CURLcode curlCode;

    if (this->curl == NULL)
        goto clean;

    inFile.open(fileName, ofstream::binary);

    if (!inFile.is_open()){
        cout << "Error while opening " << fileName << " for upload.";
        goto clean;
    }

    headers = curl_slist_append(headers, "Expect:");
    if (headers == NULL){
        cout << "Error while overwriting \"Expect\" header for upload.";
        goto clean;
    }

    headers = curl_slist_append(headers, "Content-Type: application/octet-stream");
    if (headers == NULL){
        cout << "Error while adding \"Content-Type\" header for upload.";
        goto clean;
    }

    curl_easy_setopt(this->curl, CURLOPT_READFUNCTION, UploadFile_ReadDataCallback);
    curl_easy_setopt(this->curl, CURLOPT_READDATA, (void *)&inFile);
    curl_easy_setopt(this->curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(this->curl, CURLOPT_URL, (char *)url.c_str());

    inFile.seekg(0, ios_base::end);
    if (inFile.bad() || inFile.fail()){
        cout << "IO Error on file to upload: seekg()." << endl;
        goto clean;
    }

    fileSize = inFile.tellg();

    if (fileSize == -1){
        cout << "IO Error on file to upload: tellg()." << endl;
        goto clean;
    }

    inFile.seekg(0);
    if (inFile.bad() || inFile.fail()){
        cout << "IO Error on file to upload: seekg()." << endl;
        goto clean;
    }

    curl_easy_setopt(this->curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)fileSize);
    curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, headers);

    curlCode = curl_easy_perform(this->curl);

    if (curlCode != CURLE_OK){
        cout << "Error while performing upload request for " << fileName << " (" << curl_easy_strerror(curlCode) << ")" << endl;
        goto clean;
    }

    if (curl_easy_getinfo(this->curl, CURLINFO_RESPONSE_CODE, &code) != CURLE_OK || code < 200 || code >= 300){
        cout << "Bad response code after uploading " << fileName << " (" << code << ")" << endl;
        goto clean;
    }

    ret = true;

clean:

    if (headers != NULL)
        curl_slist_free_all(headers);

    if (inFile.is_open())
        inFile.close();

    if (this->curl != NULL)
        curl_easy_reset(this->curl);

    return ret;
};
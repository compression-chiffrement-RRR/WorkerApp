#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "Process.h"

using json = nlohmann::json;
using namespace std;

class UploadMessage {

    private:

    string fileUrl;
    string responseUrl;
    string uploadUrl;
    string fileID;
    vector<unique_ptr<Process>> processes;

    public:

    void Treat();
    void Success();
    void Fail(string& error);

    UploadMessage(json& body);
    ~UploadMessage();

    private:
    bool SendResponse(json& body);
};
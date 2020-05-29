#pragma once

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;


class UploadMessage {

    private:

    string fileUrl;
    //vector<Process> processes;

    public:

    UploadMessage(json body);
    ~UploadMessage();
};
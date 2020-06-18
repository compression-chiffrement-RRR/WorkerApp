#include <gtest/gtest.h>
#include <curl/curl.h>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Environment : public ::testing::Environment {
 
  public:

  void SetUp() override {
      curl_global_init(CURL_GLOBAL_ALL);
  }

  void TearDown() override {
      curl_global_cleanup();
  }

};
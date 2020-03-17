#include "../src/include/curl_client.h"
#include <gtest/gtest.h>
#include <string>

const std::string HEADER_GET_URL = "https://postman-echo.com/headers";
const std::string GET_URL = "https://postman-echo.com/get?foo1=bar1&foo2=bar2";
const std::string POST_URL = "https://postman-echo.com/post";
const std::string DELETE_URL = "https://postman-echo.com/delete";
std::string actual_resp = "";

class CurlClientTest : public testing::Test {
public:
  SimpleCURLClient::CurlClient *curl_client;

  virtual void SetUp() {
    curl_client = new SimpleCURLClient::CurlClient(GET_URL);
  }

  virtual void TearDown() { delete curl_client; }
};

TEST_F(CurlClientTest, GetCurlUrl) {
  EXPECT_EQ(GET_URL, curl_client->getCurlUrl());
}

TEST_F(CurlClientTest, SetCurlUrl) {
  curl_client->setCurlUrl(HEADER_GET_URL);
  EXPECT_EQ(HEADER_GET_URL, curl_client->getCurlUrl());
  curl_client->setCurlUrl(GET_URL);
  EXPECT_EQ(GET_URL, curl_client->getCurlUrl());
}

TEST_F(CurlClientTest, GETRequest) {
  curl_client->setCurlUrl(GET_URL);
  std::pair<CURLcode, std::string> curl_resp = curl_client->sendGETRequest();

  // CURLE_OK means the get request went through, its equal to 0
  EXPECT_EQ(0, curl_resp.first);

  actual_resp =
      "{\"args\":{\"foo1\":\"bar1\",\"foo2\":\"bar2\"},\"headers\":{\"x-"
      "forwarded-proto\":\"https\",\"host\":\"postman-echo.com\",\"accept\":\"*"
      "/*\",\"x-forwarded-port\":\"443\"},\"url\":\"https://postman-echo.com/"
      "get?foo1=bar1&foo2=bar2\"}";

  EXPECT_EQ(actual_resp, curl_resp.second);
}

TEST_F(CurlClientTest, SetOption) {
  curl_client->setOption(CURLOPT_URL, HEADER_GET_URL);
  EXPECT_EQ(curl_client->getCurlUrl(), HEADER_GET_URL);

  std::pair<CURLcode, std::string> curl_resp = curl_client->sendGETRequest();

  EXPECT_EQ(0, curl_resp.first);

  actual_resp = "{\"headers\":{\"x-forwarded-proto\":\"https\",\"host\":"
                "\"postman-echo.com\",\"accept\":\"*\/*\",\"my-sample-header\":"
                "\"Lorem ipsum dolor sit amet\",\"x-forwarded-port\":\"443\"}}";

  // Should get an error because we didn't set headers
  EXPECT_NE(actual_resp, curl_resp.second);
}

TEST_F(CurlClientTest, SetHeader) {
  curl_client->setCurlUrl(HEADER_GET_URL);
  curl_client->setHeader({"my-sample-header: Lorem ipsum dolor sit amet"});
  std::pair<CURLcode, std::string> curl_resp = curl_client->sendGETRequest();

  // CURLE_OK means the get request went through, its equal to 0
  EXPECT_EQ(0, curl_resp.first);

  actual_resp = "{\"headers\":{\"x-forwarded-proto\":\"https\",\"host\":"
                "\"postman-echo.com\",\"accept\":\"*\/*\",\"my-sample-header\":"
                "\"Lorem ipsum dolor sit amet\",\"x-forwarded-port\":\"443\"}}";

  EXPECT_EQ(actual_resp, curl_resp.second);
}

TEST_F(CurlClientTest, POSTRequest) {
  curl_client->setCurlUrl(POST_URL);
  std::pair<CURLcode, std::string> curl_resp =
      curl_client->sendPOSTRequest("hello");

  // CURLE_OK means the get request went through, its equal to 0
  EXPECT_EQ(0, curl_resp.first);

  actual_resp =
      "{\"args\":{},\"data\":\"\",\"files\":{},\"form\":{\"hello\":\"\"},"
      "\"headers\":{\"x-forwarded-proto\":\"https\",\"host\":\"postman-echo."
      "com\",\"content-length\":\"5\",\"accept\":\"*/"
      "*\",\"content-type\":\"application/"
      "x-www-form-urlencoded\",\"x-forwarded-port\":\"443\"},\"json\":{"
      "\"hello\":\"\"},\"url\":\"https://postman-echo.com/post\"}";

  EXPECT_EQ(actual_resp, curl_resp.second);
}

TEST_F(CurlClientTest, DELETERequest) {
  curl_client->setCurlUrl(DELETE_URL);
  std::pair<CURLcode, std::string> curl_resp =
      curl_client->sendDELETERequest("delete_req");

  // CURLE_OK means the get request went through, its equal to 0
  EXPECT_EQ(0, curl_resp.first);

  actual_resp =
      "{\"args\":{},\"data\":\"\",\"files\":{},\"form\":{\"delete_req\":\"\"},"
      "\"headers\":{\"x-forwarded-proto\":\"https\",\"host\":\"postman-echo."
      "com\",\"content-length\":\"10\",\"accept\":\"*/"
      "*\",\"content-type\":\"application/"
      "x-www-form-urlencoded\",\"x-forwarded-port\":\"443\"},\"json\":{"
      "\"delete_req\":\"\"},\"url\":\"https://postman-echo.com/delete\"}";

  EXPECT_EQ(actual_resp, curl_resp.second);
}

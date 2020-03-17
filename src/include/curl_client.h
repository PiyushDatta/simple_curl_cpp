#include <curl/curl.h>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#ifndef UTILS_CURLCLIENT_H
#define UTILS_CURLCLIENT_H

namespace SimpleCURLClient {
class CurlClient {
public:
  // Initialize our curl client, set some default options if they aren't given.
  CurlClient(std::string remote_url, int ip_protocol = 1, int timeout = 10,
             bool follow_redirects = 1);
  //  Clean up curl client.
  ~CurlClient();
  // Change url target for curl client.
  void setCurlUrl(const std::string &new_url);
  // Get url that curl client is trying to access.
  std::string getCurlUrl();
  // Set an option for our curl client.
  void setOption(CURLoption curl_option_command, long curl_option_value);
  void setOption(CURLoption curl_option_command, std::string curl_option_value);
  // Set header(s) for our curl request.
  void setHeader(const std::vector<std::string> &header_list);
  // GET
  std::pair<CURLcode, std::string> sendGETRequest();
  // POST
  std::pair<CURLcode, std::string>
  sendPOSTRequest(const std::string &post_params);
  // DELETE
  std::pair<CURLcode, std::string>
  sendDELETERequest(const std::string &post_params);

private:
  std::string m_curl_url;
  CURL *m_curl;
  struct curl_slist *m_curl_header_list;

  // Make a request to the set url (m_curl_url).
  std::pair<CURLcode, std::string> makeRequest();
  // Make a request with post/delete parameters
  std::pair<CURLcode, std::string> makeRequest(const std::string &post_params);
};
} // namespace SimpleCURLClient

#endif // UTILS_CURLCLIENT_H

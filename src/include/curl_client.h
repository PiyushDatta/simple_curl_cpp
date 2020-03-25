#include <curl/curl.h>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#ifndef UTILS_CURLCLIENT_H
#define UTILS_CURLCLIENT_H

namespace simple_curlclient {
class CurlClient {
public:
  // Initialize our curl client, set some default options if they aren't given.
  CurlClient(const std::string &remote_url, const int &ip_protocol = 1,
             const int &timeout = 10, const bool &follow_redirects = 1,
             const bool &debug = false);
  // Clean up curl client.
  ~CurlClient();
  // setters and getters for the url target for curl client.
  void setCurlUrl(const std::string &new_url);
  std::string getCurlUrl();
  // Set an option for our curl client.
  void setOption(const CURLoption &curl_option_command,
                 const long &curl_option_value);
  void setOption(const CURLoption &curl_option_command,
                 const std::string &curl_option_value);
  // Set header(s) for our curl request.
  void setHeaders(const std::vector<std::string> &header_list);
  // GET
  std::pair<CURLcode, std::string> sendGETRequest();
  // POST
  std::pair<CURLcode, std::string>
  sendPOSTRequest(const std::string &post_params);
  // DELETE
  std::pair<CURLcode, std::string>
  sendDELETERequest(const std::string &delete_params);

private:
  std::string m_curl_url;
  CURL *m_curl;
  struct curl_slist *m_curl_header_list;
  bool m_debug;

  // Make a request to the set url (m_curl_url).
  std::pair<CURLcode, std::string> makeRequest();
  // Make a request with post/delete parameters
  std::pair<CURLcode, std::string> makeRequest(const std::string &post_params);
};
} // namespace simple_curlclient

#endif // UTILS_CURLCLIENT_H

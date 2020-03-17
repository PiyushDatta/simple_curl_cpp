#include "./include/curl_client.h"

namespace SimpleCURLClient {

// Custom writer function for the curl request.
std::size_t curlWriterFunction(void *contents, std::size_t size,
                               std::size_t nmemb, void *userdata) {

  size_t len = size * nmemb;
  try {
    reinterpret_cast<std::string *>(userdata)->append(
        reinterpret_cast<const char *>(contents), len);
    return len;
  } catch (std::bad_alloc &ba) {
    std::cout << "Custom curl writer function failed for request: " << ba.what()
              << std::endl;
    return 0;
  }
}

CurlClient::CurlClient(std::string remote_url, int ip_protocol, int timeout,
                       bool follow_redirects) {
  m_curl_url = remote_url;
  m_curl_header_list = nullptr;
  curl_global_init(CURL_GLOBAL_ALL);
  m_curl = curl_easy_init();

  if (m_curl) {
    // start remote url
    curl_easy_setopt(m_curl, CURLOPT_URL, remote_url.c_str());

    // don't bother trying IPv6, which would increase DNS resolution time.
    curl_easy_setopt(m_curl, CURLOPT_IPRESOLVE, ip_protocol);

    // set timeout for 10 seconds
    curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, timeout);

    // follow redirects
    curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, follow_redirects);

    std::cout << "Successfully initialized curl client" << std::endl;
  } else {
    std::cout << "Error: Could not initialize curl" << std::endl;
    this->~CurlClient();
    throw std::bad_exception();
  }
}

CurlClient::~CurlClient() {
  std::cout << "Cleaning up curl client \n" << std::endl;
  curl_slist_free_all(m_curl_header_list);
  curl_easy_cleanup(m_curl);
  curl_global_cleanup();
  m_curl = nullptr;
  m_curl_header_list = nullptr;
}

void CurlClient::setCurlUrl(const std::string &new_url) {
  m_curl_url = new_url;
  // change remote url
  setOption(CURLOPT_URL, m_curl_url.c_str());
}

std::string CurlClient::getCurlUrl() { return m_curl_url; }

void CurlClient::setOption(CURLoption curl_option_command,
                           long curl_option_value) {
  curl_easy_setopt(m_curl, curl_option_command, curl_option_value);
};

void CurlClient::setOption(CURLoption curl_option_command,
                           std::string curl_option_value) {
  if (curl_option_command == CURLOPT_URL)
    m_curl_url = curl_option_value;
  curl_easy_setopt(m_curl, curl_option_command, curl_option_value.c_str());
};

void CurlClient::setHeader(const std::vector<std::string> &header_list) {
  for (std::string hdr : header_list) {
    m_curl_header_list = curl_slist_append(m_curl_header_list, hdr.c_str());
  }
  curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_curl_header_list);
};

std::pair<CURLcode, std::string> CurlClient::makeRequest() {
  CURLcode curl_resp_code = CURLE_FAILED_INIT;
  std::string http_resp_data = "";
  try {
    // hook up data handling function.
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, &curlWriterFunction);

    // hook up data container (will be passed as the last parameter to the
    // callback handling function), can be any pointer type (passed as void
    // pointer)
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &http_resp_data);

    // run http get request and get the response code
    curl_resp_code = curl_easy_perform(m_curl);
    return std::make_pair(curl_resp_code, http_resp_data);
  } catch (int e) {
    std::cout << "Something went wrong when making a request: " << e
              << std::endl;
    return std::make_pair(curl_resp_code, http_resp_data);
  }
}

std::pair<CURLcode, std::string>
CurlClient::makeRequest(const std::string &post_params) {
  curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, post_params.c_str());
  return makeRequest();
}

std::pair<CURLcode, std::string> CurlClient::sendGETRequest() {
  std::cout << "\n"
            << "Sending GET request to: " << m_curl_url << "\n"
            << std::endl;

  return makeRequest();
}

std::pair<CURLcode, std::string>
CurlClient::sendPOSTRequest(const std::string &post_params) {
  std::cout << "\n"
            << "Sending POST request to: " << m_curl_url << std::endl;
  std::cout << "with POST params: " << post_params << "\n" << std::endl;

  return makeRequest(post_params);
}

std::pair<CURLcode, std::string>
CurlClient::sendDELETERequest(const std::string &post_params) {
  std::cout << "\n"
            << "Sending DELETE request to: " << m_curl_url << std::endl;
  std::cout << "with DELETE params: " << post_params << "\n" << std::endl;

  setOption(CURLOPT_CUSTOMREQUEST, "DELETE");
  return makeRequest(post_params);
}
} // namespace SimpleCURLClient

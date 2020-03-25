#include "./include/curl_client.h"

namespace simple_curlclient {

// Custom writer function for the curl request.
std::size_t curlWriterFunction(void *contents, std::size_t size,
                               std::size_t nmemb, void *userdata) {

  size_t len = size * nmemb;
  try {
    reinterpret_cast<std::string *>(userdata)->append(
        reinterpret_cast<const char *>(contents), len);
    return len;
  } catch (std::bad_alloc &ba) {
    std::cerr << "ERROR: Custom curl writer function failed for request: "
              << ba.what() << std::endl;
    return 0;
  }
}

CurlClient::CurlClient(const std::string &remote_url, const int &ip_protocol,
                       const int &timeout, const bool &follow_redirects,
                       const bool &debug) {
  m_curl_url = remote_url;
  m_curl_header_list = nullptr;
  curl_global_init(CURL_GLOBAL_ALL);
  m_curl = curl_easy_init();
  m_debug = debug;

  if (m_curl) {
    // start remote url
    curl_easy_setopt(m_curl, CURLOPT_URL, remote_url.c_str());

    // don't bother trying IPv6, which would increase DNS resolution time.
    curl_easy_setopt(m_curl, CURLOPT_IPRESOLVE, ip_protocol);

    // set timeout for 10 seconds
    curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, timeout);

    // follow redirects
    curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, follow_redirects);

    if (m_debug) {
      std::cout << "Successfully initialized curl client" << '\n';
    }
  } else {
    std::cerr << "Error: Could not initialize curl" << '\n';
    this->~CurlClient();
    throw std::bad_exception();
  }
}

CurlClient::~CurlClient() {
  if (m_debug) {
    std::cout << "Cleaning up curl client" << '\n';
  }
  curl_slist_free_all(m_curl_header_list);
  curl_easy_cleanup(m_curl);
  curl_global_cleanup();
  m_curl = nullptr;
  m_curl_header_list = nullptr;
}

void CurlClient::setCurlUrl(const std::string &new_url) {
  m_curl_url = new_url;
  setOption(CURLOPT_URL, m_curl_url.c_str());
}

std::string CurlClient::getCurlUrl() { return m_curl_url; }

void CurlClient::setOption(const CURLoption &curl_option_command,
                           const long &curl_option_value) {
  curl_easy_setopt(m_curl, curl_option_command, curl_option_value);
};

void CurlClient::setOption(const CURLoption &curl_option_command,
                           const std::string &curl_option_value) {
  if (curl_option_command == CURLOPT_URL) {
    m_curl_url = curl_option_value;
  }
  curl_easy_setopt(m_curl, curl_option_command, curl_option_value.c_str());
};

void CurlClient::setHeaders(const std::vector<std::string> &header_list) {
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
    std::cerr << "Something went wrong when making a request: " << e << '\n';
    return std::make_pair(curl_resp_code, http_resp_data);
  }
}

std::pair<CURLcode, std::string>
CurlClient::makeRequest(const std::string &post_params) {
  curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, post_params.c_str());
  return makeRequest();
}

std::pair<CURLcode, std::string> CurlClient::sendGETRequest() {
  if (m_debug) {
    std::cout << "Sending GET request to: " << m_curl_url << "\n";
  }
  return makeRequest();
}

std::pair<CURLcode, std::string>
CurlClient::sendPOSTRequest(const std::string &post_params) {
  if (m_debug) {
    std::cout << "Sending POST request to: " << m_curl_url << '\n';
    std::cout << "with POST params: " << post_params << "\n";
  }
  return makeRequest(post_params);
}

std::pair<CURLcode, std::string>
CurlClient::sendDELETERequest(const std::string &delete_params) {
  if (m_debug) {
    std::cout << "Sending DELETE request to: " << m_curl_url << '\n';
    std::cout << "with DELETE params: " << delete_params << "\n";
  }
  setOption(CURLOPT_CUSTOMREQUEST, "DELETE");
  return makeRequest(delete_params);
}
} // namespace simple_curlclient

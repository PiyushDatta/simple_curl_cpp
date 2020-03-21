cmake_minimum_required(VERSION 3.10.2)

set(SOURCES
    curl_client.cc    
)

add_library(simple_curl_cpp ${SOURCES})
target_include_directories(simple_curl_cpp PUBLIC include PRIVATE)
target_link_libraries(simple_curl_cpp PUBLIC libcurl)

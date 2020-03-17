cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
    curl_client_test.cc
    ../src/curl_client.cc   
)

set(HEADERS
    ../src/include/curl_client.h
)

# googletest
enable_testing()

add_executable(simple_curl_cpp_test "main.cc" ${SOURCES} ${HEADERS})
target_link_libraries(simple_curl_cpp_test libcurl gtest)

add_test(
    NAME simple_curl_cpp_test
    COMMAND simple_curl_cpp_test
)

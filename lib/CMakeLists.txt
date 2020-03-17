cmake_minimum_required(VERSION 3.10.2)

message(STATUS "Not using system Curl, using built-in curl project instead.")
option(BUILD_TESTING "Set to ON to build cURL tests." OFF)
option(BUILD_CURL_EXE "Set to ON to build cURL executable." OFF)

if (WIN32)
    # only need to enable
    message("On windows, using WinSSL.")
    option(CMAKE_USE_WINSSL "Set to ON to use WinSSL for windows." ON)
else()
    option(CMAKE_USE_WINSSL "Set to ON to use WinSSL for windows." OFF)
endif (WIN32)

add_subdirectory(curl)

set(gtest_force_shared_crt TRUE CACHE INTERNAL "")
set(BUILD_GMOCK TRUE CACHE INTERNAL "")
set(INSTALL_GTEST FALSE)
set(gmock_build_tests FALSE CACHE INTERNAL "")
add_subdirectory(googletest)

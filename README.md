# Curl Wrapper

Just a basic wrapper around curl for c++

# Dependencies

1. `cmake >= 3.10.2`
2. For linux, openssl >= 1.1.1, on ubuntu you can do `sudo apt-get install libssl-dev`
3. For linux, if you might also need `sudo apt-get install build-essential` for building with cmake
4. `python (>= 2.7)`

# Cloning

1. `git clone --recursive https://github.com/PiyushDatta/simple_curl_cpp`

   OR

1. `git clone https://github.com/PiyushDatta/simple_curl_cpp`
2. `git submodule init`
3. `git submodule update`

# Building with shell script

1. Make sure permissions are okay for shell script by doing `ls -l ./build.sh`, otherwise just do `chmod 775 ./build.sh`
2. Build with cmake && make: `./build.sh --make`
3. Build with cmake && cmake --build . (windows mostly): `./build.sh`
4. Run tests: `-rt`
5. Run verbose/debug mode: `-v`
6. Run without building (running any cmake or make command): `-nb`
7. Build only tests: `bt`
8. Buld clean: `bc`
9. Help: `-h`

# Building on your own with cmake

1. Go into the simple_curl_cpp directory
2. `mkdir -p build`
3. `cd build`
4. `cmake ..`
5. If on linux: `make`
6. If on windows: `cmake --build .`
7. Optional, if you want to only build test files: `cmake --build . --target simple_curl_cpp_test`

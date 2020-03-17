#!/usr/bin/env bash

# arguments
build_only_tests=false
build_clean=""
run_tests=false
no_build_project=false
cmake_debug=false
use_make=false

for arg in "$@"; do
    if [ "$arg" == "--build_clean" ] || [ "$arg" == "-bc" ]; then
        build_clean="cmake .." && echo "Building project without cache (clean build)."
    fi
    if [ "$arg" == "--build_only_tests" ] || [ "$arg" == "-bt" ]; then
        build_only_tests=true && echo "Building only the test files."
    fi
    if [ "$arg" == "--run_tests" ] || [ "$arg" == "-rt" ]; then
        run_tests=true && echo "Running tests."
    fi
    if [ "$arg" == "--no_build_project" ] || [ "$arg" == "-nb" ]; then
        no_build_project=true && echo "Not building project."
    fi
    if [ "$arg" == "--use_make" ] || [ "$arg" == "--make" ] || [ "$arg" == "-m" ]; then
        use_make=true && echo "Using make instead of cmake --build."
    fi
    if [ "$arg" == "--debug" ] || [ "$arg" == "--verbose" ] || [ "$arg" == "-d" ] ||
        [ "$arg" == "-v" ]; then
        cmake_debug=true && echo "Cmake build with --verbose (debug)."
    fi
    if [ "$arg" == "--help" ] || [ "$arg" == "-h" ]; then
        echo "Help argument detected."
    fi
done

# construct build command
if [ "$use_make" = false ]; then
    # build everything using cmake
    echo "Using cmake and cmake build." && cmake_build_cmd="cmake --build ."
else
    echo "Using cmake and make." && cmake_build_cmd="make"
fi

# build everything with clean
if [ "$build_clean" = "cmake .." ]; then
    if [ "$use_make" = false ]; then
        cmake_build_cmd="${cmake_build_cmd} --target clean"
    else
        cmake_build_cmd="${cmake_build_cmd} clean"
    fi
fi

# build tests
if [ "$use_make" = false ] && [ "$build_only_tests" = true ] && [ -z "$build_clean" ]; then
    cmake_build_cmd="${cmake_build_cmd} --target simple_curl_cpp_test"
fi

# build with debug/verbose
if [ "$cmake_debug" = true ]; then
    if [ "$use_make" = false ]; then
        cmake_build_cmd="${cmake_build_cmd} --verbose"
    else
        cmake_build_cmd="${cmake_build_cmd} -n"
    fi
fi

# make a build folder, if not present
if [ -d "build" ]; then
    echo "Build directory already present, not doing cmake .."
    build_clean=""
else
    mkdir build
    build_clean="cmake .."
    echo "Created build directory"
fi

# go to build folder
cd build

# build the entire project
if [ "$no_build_project" = false ]; then
    echo "Making simple_curl_cpp project, running makefile generator command: "
    echo ${build_clean}
    ${build_clean}
    echo "Building simple_curl_cpp, running command: "
    echo ${cmake_build_cmd}
    ${cmake_build_cmd}
    echo "FINISHED BUILD SCRIPTS"
fi

# Run the test files
if [ "$run_tests" = true ]; then
    echo "Running tests for simple_curl_cpp"
    if [ -f "./test/Debug/simple_curl_cpp_test" ]; then
        ./test/Debug/simple_curl_cpp_test
    elif [ -f "./test/simple_curl_cpp_test" ]; then
        ./test/simple_curl_cpp_test
    else
        echo "Can't find test files, please try rebuilding."
    fi
fi

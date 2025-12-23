cmake_minimum_required(VERSION 3.10)
project(ntasm LANGUAGES CXX)

if(MSVC)
    message(FATAL_ERROR
            "MSVC detected as the compiler, which is not supported.\n"
            "Please reconfigure with CMake to use GCC/G++ or Clang.\n"
            "Once you've installed one of these compiler suites, the\n"
            "easiest way to do this on Windows is to run the\n"
            "build.bat file in the TeenyAT root directory.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/bin")

add_executable(ntasm
    errorlog.cpp
    lexer.cpp
    preprocessor.cpp
    parser.cpp
    main.cpp
)

set(WARNING_OPTIONS -Wall -Wextra -Wpedantic)

target_compile_options(ntasm PRIVATE ${WARNING_OPTIONS})

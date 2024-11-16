cmake_minimum_required(VERSION 3.11)
project(chash)

# Enable C++ standard
set(CMAKE_CXX_STANDARD 11)

# Include FetchContent module
include(FetchContent)

# Fetch Catch2
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.7.1
)

FetchContent_MakeAvailable(Catch2)

add_executable(chash_tests test.cpp)

# Link Catch2 to your test executable
target_link_libraries(chash_tests PRIVATE Catch2::Catch2WithMain)

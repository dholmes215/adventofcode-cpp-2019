cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(adventofcode-cpp-2019 LANGUAGES CXX)

if(MSVC)
  add_compile_options(/W4 /WX)
else(MSVC)
  add_compile_options(-Wall -Wextra)
#  Conan+Boost is choking on -Werror
#  add_compile_options(-Wall -Wextra -Werror)
endif(MSVC)

# Use Conan to install dependencies
# conan.cmake comes from https://github.com/conan-io/cmake-conan
include(conan.cmake)
conan_cmake_run(CONANFILE conanfile.txt
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing)

# Configure Conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(src)


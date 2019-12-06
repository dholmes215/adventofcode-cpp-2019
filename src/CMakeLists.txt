add_library(intcode STATIC intcode.cpp intcode.hpp)
set_target_properties(intcode
  PROPERTIES
    CXX_STANDARD 20
    CXX_EXTENSIONS OFF
    CXX_STANARD_REQUIRED ON
    CXX_CLANG_TIDY "clang-tidy;-warnings-as-errors=*"
  )

add_executable(intcodetest intcodetest.cpp intcode)
set_target_properties(intcodetest
  PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    CXX_STANARD_REQUIRED ON
    CXX_CLANG_TIDY "clang-tidy;-warnings-as-errors=*"
)
target_link_libraries(intcodetest ${CONAN_LIBS})


add_executable(day01 day01.cpp)
set_target_properties(day01
  PROPERTIES
    CXX_STANDARD 20
    CXX_EXTENSIONS OFF
    CXX_STANARD_REQUIRED ON
    CXX_CLANG_TIDY "clang-tidy;-warnings-as-errors=*"
  )


add_executable(day02 day02.cpp intcode)
set_target_properties(day02
  PROPERTIES
    CXX_STANDARD 20
    CXX_EXTENSIONS OFF
    CXX_STANARD_REQUIRED ON
    CXX_CLANG_TIDY "clang-tidy;-warnings-as-errors=*"
  )

add_executable(day05 day05.cpp intcode)
set_target_properties(day05
  PROPERTIES
    CXX_STANDARD 20
    CXX_EXTENSIONS OFF
    CXX_STANARD_REQUIRED ON
    CXX_CLANG_TIDY "clang-tidy;-warnings-as-errors=*"
  )

cmake_minimum_required(VERSION 3.13)
project(huffman)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fsanitize=address -fno-omit-frame-pointer")
set(LSAN_OPTIONS=verbosity=1:log_threads=1)

add_executable(huffman huffman.cpp main.cpp)

set(MULTIPLY_TIMEOUT 1)

enable_testing()
add_test(Test_5M_0
        huffman -v -c "smoke_test/5M_0.in" "smoke_test/5M_0.out"
        huffman -v -d "smoke_test/5M_0.out" "/tmp/5M_0.in"
        diff -q smoke_test/5M_0.in /tmp/5M_0.in || echo "error"
        )

set_tests_properties(
        PROPERTIES TIMEOUT ${MULTIPLY_TIMEOUT})

set_tests_properties(PROPERTIES
        PASS_REGULAR_EXPRESSION ""
        FAIL_REGULAR_EXPRESSION "error")

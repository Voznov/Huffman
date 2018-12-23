# CMake generated Testfile for 
# Source directory: /home/admin/Projects/Huffman
# Build directory: /home/admin/Projects/Huffman
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(Test_5M_0 "huffman" "-v" "-c" "smoke_test/5M_0.in" "smoke_test/5M_0.out" "huffman" "-v" "-d" "smoke_test/5M_0.out" "/tmp/5M_0.in" "diff" "-q" "smoke_test/5M_0.in" "/tmp/5M_0.in" "||" "echo" "error")

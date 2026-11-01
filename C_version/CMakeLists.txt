cmake_minimum_required(VERSION 3.10.0)
project(crosswords_C_version VERSION 0.1.0 LANGUAGES C)

add_executable(crosswords_C_version main.c sort_words.c)

add_executable(sorting_test sort_words.c wordlist_tests.c)

add_test(NAME sort_test COMMAND sorting_test)

include(CTest)
enable_testing()


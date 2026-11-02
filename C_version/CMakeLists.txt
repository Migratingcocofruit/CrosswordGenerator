cmake_minimum_required(VERSION 3.10.0)
project(crosswords_C_version VERSION 0.1.0 LANGUAGES C)

add_executable(crosswords_C_version main.c sort_words.c puzzle_globals.c generate_puzzle.c)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(crosswords_C_version PRIVATE Threads::Threads)

add_executable(sorting_test sort_words.c wordlist_tests.c puzzle_globals.c)

add_test(NAME sort_test COMMAND sorting_test)

add_test(NAME generate_board COMMAND crosswords_C_version)

include(CTest)
enable_testing()


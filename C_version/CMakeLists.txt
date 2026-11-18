cmake_minimum_required(VERSION 3.10.0)
project(crossword_generator VERSION 0.1.0 LANGUAGES C)

add_executable(crossword_generator main.c sort_words.c puzzle_globals.c generate_puzzle.c)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(crossword_generator PRIVATE Threads::Threads)

add_executable(sorting_test sort_words.c wordlist_tests.c puzzle_globals.c)

add_test(NAME sort_test COMMAND sorting_test)

add_test(NAME generate_board COMMAND crossword_generator)

include(CTest)
enable_testing()


cmake_minimum_required(VERSION 3.15)
project(fishing_simulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# ── Game library (shared between app and tests) ───────────────────────────
add_library(game_lib src/Game.cpp)
target_include_directories(game_lib PUBLIC include)
target_compile_options(game_lib PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Werror -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
)

# ── Main executable ───────────────────────────────────────────────────────
add_executable(fishing_simulator src/main.cpp)
target_link_libraries(fishing_simulator PRIVATE game_lib)

# ── Google Test ───────────────────────────────────────────────────────────
include(FetchContent)
set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE NEVER)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
if(MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(fishing_simulator_tests tests/test_game.cpp)
target_link_libraries(fishing_simulator_tests PRIVATE game_lib gtest_main)
target_include_directories(fishing_simulator_tests PRIVATE include)
target_compile_options(fishing_simulator_tests PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Werror -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
)

include(GoogleTest)
gtest_discover_tests(fishing_simulator_tests)

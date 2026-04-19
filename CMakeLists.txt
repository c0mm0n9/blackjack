cmake_minimum_required(VERSION 3.16)
project(blackjack LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BJ_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(
    BJ_SOURCES
    src/Card.cpp
    src/Hand.cpp
    src/Deck.cpp
    src/Player.cpp
    src/HumanPlayer.cpp
    src/Dealer.cpp
    src/GameManager.cpp
)

add_library(bjack STATIC ${BJ_SOURCES})
target_include_directories(bjack PUBLIC "${BJ_INCLUDE}")

add_executable(blackjack src/main.cpp)
target_link_libraries(blackjack PRIVATE bjack)

option(BJ_BUILD_TESTS "Build Catch2 unit tests" ON)

if(BJ_BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.7.1
    )
    FetchContent_MakeAvailable(Catch2)

    add_executable(
        blackjack_tests
        tests/test_difficulty.cpp
        tests/test_card.cpp
        tests/test_deck.cpp
        tests/test_hand.cpp
        tests/test_player.cpp
        tests/test_human_player.cpp
        tests/test_dealer.cpp
        tests/test_game_manager.cpp
        tests/test_integration.cpp
        tests/test_nonfunctional.cpp
    )
    target_link_libraries(blackjack_tests PRIVATE bjack Catch2::Catch2WithMain)

    include(CTest)
    add_test(NAME blackjack_tests COMMAND blackjack_tests)
endif()

#include <catch2/catch_test_macros.hpp>

// Planned cases: tests/unit_test_plan.md — section 6 `HumanPlayer`

TEST_CASE("HP-01: Initial chips", "[human_player][HP-01]") {}

TEST_CASE("HP-02: addChips", "[human_player][HP-02]") {}

TEST_CASE("HP-03: placeBet legal", "[human_player][HP-03]") {}

TEST_CASE("HP-04: placeBet insufficient", "[human_player][HP-04]") {}

TEST_CASE("HP-05: makeDecision with scripted input", "[human_player][HP-05]") {}

TEST_CASE("HP-06: getScore delegates to hand", "[human_player][HP-06]") {}

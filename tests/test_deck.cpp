#include <catch2/catch_test_macros.hpp>

// Planned cases: tests/unit_test_plan.md — section 3 `Deck`

TEST_CASE("DK-01: Construction size", "[deck][DK-01]") {}

TEST_CASE("DK-02: Construction invalid input", "[deck][DK-02]") {}

TEST_CASE("DK-03: Shuffle changes order", "[deck][DK-03]") {}

TEST_CASE("DK-04: Draw consumes card", "[deck][DK-04]") {}

TEST_CASE("DK-05: Draw empty deck", "[deck][DK-05]") {}

TEST_CASE("DK-06: Destructor cleanup", "[deck][DK-06]") {}

TEST_CASE("DK-07: Resource safety after move (if Deck becomes movable)", "[deck][DK-07]") {}

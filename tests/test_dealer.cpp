#include <catch2/catch_test_macros.hpp>

// Planned cases: tests/unit_test_plan.md — section 7 `Dealer`
// Assertions intentionally omitted until dealer rules are implemented.

TEST_CASE("DL-01: makeDecision hits soft 17 (if rule)", "[dealer][DL-01]") {}

TEST_CASE("DL-02: makeDecision stands hard 17+", "[dealer][DL-02]") {}

TEST_CASE("DL-03: revealHiddenCard", "[dealer][DL-03]") {}

TEST_CASE("DL-04: Difficulty field", "[dealer][DL-04]") {}

TEST_CASE("DL-05: getScore", "[dealer][DL-05]") {}

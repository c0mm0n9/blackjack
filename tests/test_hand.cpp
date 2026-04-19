#include <catch2/catch_test_macros.hpp>

// Planned cases: tests/unit_test_plan.md — section 4 `Hand`
// Assertions intentionally omitted until `Hand` / `Card` API is implemented.

TEST_CASE("H-01: Empty hand score", "[hand][H-01]") {}

TEST_CASE("H-02: Numeric totals", "[hand][H-02]") {}

TEST_CASE("H-03: Face cards", "[hand][H-03]") {}

TEST_CASE("H-04: Soft ace", "[hand][H-04]") {}

TEST_CASE("H-05: Multiple aces", "[hand][H-05]") {}

TEST_CASE("H-06: Bust detection", "[hand][H-06]") {}

TEST_CASE("H-07: clearHand", "[hand][H-07]") {}

TEST_CASE("H-08: addCard ownership", "[hand][H-08]") {}

TEST_CASE("H-09: Destructor / clear with ownership", "[hand][H-09]") {}

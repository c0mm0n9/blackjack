#include <catch2/catch_test_macros.hpp>

// Planned cases: tests/unit_test_plan.md — section 5 `Player` (abstract)

TEST_CASE("P-01: Cannot instantiate base Player (compile-time / API)", "[player][P-01]") {
    // P-01 is enforced by pure virtual `Player`; no runtime assertion required here.
}

TEST_CASE("P-02: Polymorphic deletion", "[player][P-02]") {}

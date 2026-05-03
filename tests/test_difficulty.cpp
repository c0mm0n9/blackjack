#include <catch2/catch_test_macros.hpp>

#include "Difficulty.h"
#include "GameManager.h"
#include "Deck.h"

// Planned cases: tests/unit_test_plan.md — section 1 `Difficulty`

using namespace blackjack;

TEST_CASE("D-01: Enum completeness", "[difficulty][D-01]") {
    // Enum values are distinct
    REQUIRE(Difficulty::EASY != Difficulty::NORMAL);
    REQUIRE(Difficulty::EASY != Difficulty::HARD);
    REQUIRE(Difficulty::NORMAL != Difficulty::HARD);
}

TEST_CASE("D-02: Shoe size rule", "[difficulty][D-02]") {
    GameManager gameManager;
    REQUIRE_NOTHROW(gameManager.setDifficulty(Difficulty::EASY));
    REQUIRE(gameManager.getGameDeck()->getTotalCards() == 52 * 1);
    REQUIRE_NOTHROW(gameManager.setDifficulty(Difficulty::NORMAL));
    REQUIRE(gameManager.getGameDeck()->getTotalCards() == 52 * 4);
    REQUIRE_NOTHROW(gameManager.setDifficulty(Difficulty::HARD));
    REQUIRE(gameManager.getGameDeck()->getTotalCards() == 52 * 8);
}

TEST_CASE("D-03: Blackjack payout rule (when wired)", "[difficulty][D-03]") {}

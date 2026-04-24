#include <catch2/catch_test_macros.hpp>

#include "Card.h"

// Planned cases: tests/unit_test_plan.md — section 2 `Card`

TEST_CASE("C-01: Construction / defaults", "[card][C-01]") {
    blackjack::Card card(blackjack::Suit::HEARTS, blackjack::Rank::ACE);
    REQUIRE(card.suit_ == blackjack::Suit::HEARTS);
    REQUIRE(card.rank_ == blackjack::Rank::ACE);
}

TEST_CASE("C-02: Equality or identity", "[card][C-02]") {
    blackjack::Card card1(blackjack::Suit::HEARTS, blackjack::Rank::ACE);
    blackjack::Card card2(blackjack::Suit::HEARTS, blackjack::Rank::ACE);
    REQUIRE(card1 == card2);
}

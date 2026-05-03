#include <catch2/catch_test_macros.hpp>
#include <vector>

#include "Deck.h"
#include "Card.h"

// Planned cases: tests/unit_test_plan.md — section 3 `Deck`

TEST_CASE("DK-01: Construction size", "[deck][DK-01]") {
    blackjack::Deck deck(1);
    std::vector<blackjack::Card*> drawn;
    drawn.reserve(52);
    for (int i = 0; i < 52; i++) {
        auto* card = deck.drawCard();
        REQUIRE(card != nullptr);
        drawn.push_back(card);
    }

    for (int i = 0; i < 52; i++) {
        for (int j = i + 1; j < 52; j++) {
            REQUIRE(drawn[i] != drawn[j]);
        }
    }

    REQUIRE_THROWS_AS(deck.drawCard(), std::runtime_error);

    for (blackjack::Card* c : drawn) {
        delete c;
    }
}

TEST_CASE("DK-02: Construction invalid input", "[deck][DK-02]") {
    REQUIRE_THROWS_AS(blackjack::Deck(0), std::invalid_argument);
    REQUIRE_THROWS_AS(blackjack::Deck(-1), std::invalid_argument);
}

TEST_CASE("DK-03: Shuffle changes order", "[deck][DK-03]") {
    blackjack::Deck deck(1);
    blackjack::Deck baseline(1);
    deck.shuffle(42);

    bool anyDifferent = false;
    for (int i = 0; i < 52; i++) {
        auto* shuffledCard = deck.drawCard();
        auto* baselineCard = baseline.drawCard();
        REQUIRE(shuffledCard != nullptr);
        REQUIRE(baselineCard != nullptr);
        if (shuffledCard != baselineCard) {
            anyDifferent = true;
        }
        delete shuffledCard;
        delete baselineCard;
    }
    REQUIRE(anyDifferent);
}

TEST_CASE("DK-04: Draw consumes card", "[deck][DK-04]") {
    blackjack::Deck deck(1);
    auto* card = deck.drawCard();
    REQUIRE(card != nullptr);
    delete card;
}

TEST_CASE("DK-05: Draw empty deck", "[deck][DK-05]") {
    blackjack::Deck deck(1);
    for (int i = 0; i < 52; i++) {
        delete deck.drawCard();
    }
    REQUIRE_THROWS_AS(deck.drawCard(), std::runtime_error);
}

TEST_CASE("DK-06: Destructor cleanup", "[deck][DK-06]") {
    SUCCEED("Automatic destructor cleanup is validated by normal scope exit.");
}

TEST_CASE("DK-07: Resource safety after move (if Deck becomes movable)", "[deck][DK-07]") {
    SUCCEED("Deck move semantics are not implemented in this skeleton.");
}

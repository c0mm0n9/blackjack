#include <catch2/catch_test_macros.hpp>
#include "Hand.h"
#include "Card.h"
// Planned cases: tests/unit_test_plan.md — section 4 `Hand`

TEST_CASE("H-01: Empty hand score", "[hand][H-01]") {
    blackjack::Hand hand;
    REQUIRE(hand.getScore() == 0);
    REQUIRE(!hand.isBust());
    REQUIRE(!hand.isNaturalBlackjack());
}

TEST_CASE("H-02: Numeric totals", "[hand][H-02]") {
    blackjack::Hand hand;
    hand.addCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::ACE));
    hand.addCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::TWO));
    REQUIRE(hand.getScore() == 13);
    REQUIRE(!hand.isBust());
    REQUIRE(!hand.isNaturalBlackjack());
}

TEST_CASE("H-03: Face cards", "[hand][H-03]") {    
    blackjack::Hand hand;
    hand.addCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::JACK));
    hand.addCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::QUEEN));
    hand.addCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::KING));
    REQUIRE(hand.getScore() == 30);
    REQUIRE(hand.isBust());
    REQUIRE(!hand.isNaturalBlackjack());
}

TEST_CASE("H-04: Soft ace", "[hand][H-04]") {
    blackjack::Hand hand;
    hand.addCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::ACE));
    hand.addCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::TWO));
    REQUIRE(hand.getScore() == 13);
    REQUIRE(!hand.isBust());
    REQUIRE(!hand.isNaturalBlackjack());
    hand.addCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::TEN));
    REQUIRE(hand.getScore() == 13);
    REQUIRE(!hand.isBust());
    REQUIRE(!hand.isNaturalBlackjack());
}

TEST_CASE("H-05: Multiple aces", "[hand][H-05]") {
    blackjack::Hand hand;
    hand.addCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::ACE));
    hand.addCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::ACE));
    REQUIRE(hand.getScore() == 12);
    REQUIRE(!hand.isBust());
    REQUIRE(!hand.isNaturalBlackjack());
}

TEST_CASE("H-06: Bust detection", "[hand][H-06]") {
    blackjack::Hand hand;
    hand.addCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::ACE));
    hand.addCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::TEN));
    hand.addCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::TEN));
    REQUIRE(hand.getScore() == 21);
    REQUIRE(!hand.isBust());
    REQUIRE(!hand.isNaturalBlackjack());
    hand.addCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::TEN));
    REQUIRE(hand.getScore() == 31);
    REQUIRE(hand.isBust());
    REQUIRE(!hand.isNaturalBlackjack());
}

TEST_CASE("H-07: clearHand", "[hand][H-07]") {
    blackjack::Hand hand;
    hand.addCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::TEN));
    hand.addCard(new blackjack::Card(blackjack::Suit::DIAMONDS, blackjack::Rank::FIVE));
    REQUIRE(hand.getScore() == 15);
    hand.clearHand();
    REQUIRE(hand.getScore() == 0);
    REQUIRE(!hand.isBust());
    REQUIRE(!hand.isNaturalBlackjack());
}

TEST_CASE("H-08: addCard ownership", "[hand][H-08]") {
    blackjack::Hand hand;
    REQUIRE(hand.getScore() == 0);
    hand.addCard(new blackjack::Card(blackjack::Suit::CLUBS, blackjack::Rank::SEVEN));
    REQUIRE(hand.getScore() == 7);
    hand.addCard(new blackjack::Card(blackjack::Suit::SPADES, blackjack::Rank::EIGHT));
    REQUIRE(hand.getScore() == 15);
    hand.addCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::THREE));
    REQUIRE(hand.getScore() == 18);
}

TEST_CASE("H-09: Destructor / clear with ownership", "[hand][H-09]") {
    // Verify that Hand destructor properly cleans up cards (no crash / leak)
    {
        blackjack::Hand hand;
        hand.addCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::ACE));
        hand.addCard(new blackjack::Card(blackjack::Suit::DIAMONDS, blackjack::Rank::KING));
        // Hand goes out of scope — destructor should clean up
    }
    // clearHand then add new cards — verifies no double-free
    {
        blackjack::Hand hand;
        hand.addCard(new blackjack::Card(blackjack::Suit::CLUBS, blackjack::Rank::FIVE));
        hand.clearHand();
        hand.addCard(new blackjack::Card(blackjack::Suit::SPADES, blackjack::Rank::NINE));
        REQUIRE(hand.getScore() == 9);
    }
    SUCCEED("No crash or double-free detected.");
}

#include <catch2/catch_test_macros.hpp>
#include "HumanPlayer.h"
#include "Card.h"
#include <sstream>
#include <iostream>

// Planned cases: tests/unit_test_plan.md — section 6 `HumanPlayer`

// ---------------------------------------------------------------------------
// HP-01: Starting bankroll — constructor sets initial chips correctly.
// ---------------------------------------------------------------------------
TEST_CASE("HP-01: Initial chips", "[human_player][HP-01]") {
    blackjack::HumanPlayer hp("Alice", 500);
    REQUIRE(hp.getChips() == 500);
}

TEST_CASE("HP-01: Initial chips default", "[human_player][HP-01]") {
    blackjack::HumanPlayer hp;
    REQUIRE(hp.getChips() == 100);
}

// ---------------------------------------------------------------------------
// HP-02: addChips increases the bankroll.
// ---------------------------------------------------------------------------
TEST_CASE("HP-02: addChips", "[human_player][HP-02]") {
    blackjack::HumanPlayer hp("Bob", 100);
    REQUIRE(hp.getChips() == 100);
    hp.addChips(50);
    REQUIRE(hp.getChips() == 150);
    hp.addChips(0);
    REQUIRE(hp.getChips() == 150);
    hp.addChips(1000);
    REQUIRE(hp.getChips() == 1150);
}

// ---------------------------------------------------------------------------
// HP-03: placeBet — legal bet deducts chips and returns the bet amount.
// ---------------------------------------------------------------------------
TEST_CASE("HP-03: placeBet legal", "[human_player][HP-03]") {
    blackjack::HumanPlayer hp("Carol", 200);

    // Simulate user typing "50" on stdin
    std::istringstream input("50\n");
    std::streambuf* orig = std::cin.rdbuf(input.rdbuf());

    int bet = hp.placeBet();
    std::cin.rdbuf(orig);

    REQUIRE(bet == 50);
    REQUIRE(hp.getChips() == 150);
}

// ---------------------------------------------------------------------------
// HP-04: placeBet — insufficient funds, bet is rejected.
// ---------------------------------------------------------------------------
TEST_CASE("HP-04: placeBet insufficient", "[human_player][HP-04]") {
    blackjack::HumanPlayer hp("Dave", 30);

    std::istringstream input("100\n");
    std::streambuf* orig = std::cin.rdbuf(input.rdbuf());

    int bet = hp.placeBet();
    std::cin.rdbuf(orig);

    REQUIRE(bet == 0);
    REQUIRE(hp.getChips() == 30);  // chips unchanged
}

TEST_CASE("HP-04: placeBet zero/negative rejected", "[human_player][HP-04]") {
    blackjack::HumanPlayer hp("Eve", 100);

    std::istringstream input("0\n");
    std::streambuf* orig = std::cin.rdbuf(input.rdbuf());

    int bet = hp.placeBet();
    std::cin.rdbuf(orig);

    REQUIRE(bet == 0);
    REQUIRE(hp.getChips() == 100);
}

// ---------------------------------------------------------------------------
// HP-05: makeDecision with scripted input — 'h' for hit, 's' for stand.
// ---------------------------------------------------------------------------
TEST_CASE("HP-05: makeDecision with scripted input hit", "[human_player][HP-05]") {
    blackjack::HumanPlayer hp("Frank", 100);
    hp.receiveCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::FIVE));
    hp.receiveCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::SIX));

    // Simulate 'h' input
    std::istringstream input("h\n");
    std::streambuf* origCin = std::cin.rdbuf(input.rdbuf());
    // Suppress cout output during test
    std::streambuf* origCout = std::cout.rdbuf(nullptr);

    REQUIRE_NOTHROW(hp.makeDecision());

    std::cin.rdbuf(origCin);
    std::cout.rdbuf(origCout);
}

TEST_CASE("HP-05: makeDecision with scripted input stand", "[human_player][HP-05]") {
    blackjack::HumanPlayer hp("Grace", 100);
    hp.receiveCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::TEN));
    hp.receiveCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::SEVEN));

    std::istringstream input("s\n");
    std::streambuf* origCin = std::cin.rdbuf(input.rdbuf());
    std::streambuf* origCout = std::cout.rdbuf(nullptr);

    REQUIRE_NOTHROW(hp.makeDecision());

    std::cin.rdbuf(origCin);
    std::cout.rdbuf(origCout);
}

// ---------------------------------------------------------------------------
// HP-06: getScore delegates to Hand — score matches hand composition.
// ---------------------------------------------------------------------------
TEST_CASE("HP-06: getScore delegates to hand", "[human_player][HP-06]") {
    blackjack::HumanPlayer hp("Hank", 100);

    // Empty hand — score should be 0
    REQUIRE(hp.getScore() == 0);

    // Add cards: 7 + 9 = 16
    hp.receiveCard(new blackjack::Card(blackjack::Suit::DIAMONDS, blackjack::Rank::SEVEN));
    REQUIRE(hp.getScore() == 7);

    hp.receiveCard(new blackjack::Card(blackjack::Suit::CLUBS, blackjack::Rank::NINE));
    REQUIRE(hp.getScore() == 16);
}

TEST_CASE("HP-06: getScore with ace adjustment", "[human_player][HP-06]") {
    blackjack::HumanPlayer hp("Iris", 100);

    // ACE(11) + KING(10) = 21
    hp.receiveCard(new blackjack::Card(blackjack::Suit::SPADES, blackjack::Rank::ACE));
    hp.receiveCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::KING));
    REQUIRE(hp.getScore() == 21);

    // Add 5 → ACE becomes 1 → 1 + 10 + 5 = 16
    hp.receiveCard(new blackjack::Card(blackjack::Suit::DIAMONDS, blackjack::Rank::FIVE));
    REQUIRE(hp.getScore() == 16);
}

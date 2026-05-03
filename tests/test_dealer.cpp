#include <catch2/catch_test_macros.hpp>
#include "Dealer.h"
#include "Card.h"
#include "Hand.h"

// Planned cases: tests/unit_test_plan.md — section 7 `Dealer`

// ---------------------------------------------------------------------------
// Helper: create a Dealer and deal specific cards into its hand.
// ---------------------------------------------------------------------------
static blackjack::Dealer makeDealerWith(std::initializer_list<blackjack::Card*> cards,
                                        blackjack::Difficulty level = blackjack::Difficulty::NORMAL) {
    blackjack::Dealer d(level);
    for (auto* c : cards) {
        d.receiveCard(c);
    }
    return d;
}

// ---------------------------------------------------------------------------
// DL-01: makeDecision hits on soft 17 (house rule).
// ---------------------------------------------------------------------------
TEST_CASE("DL-01: makeDecision hits soft 17 (if rule)", "[dealer][DL-01]") {
    // Soft 17 = Ace(11) + 6 = 17, hand is soft → should hit
    blackjack::Dealer dealer(blackjack::Difficulty::NORMAL);
    dealer.receiveCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::ACE));
    dealer.receiveCard(new blackjack::Card(blackjack::Suit::DIAMONDS, blackjack::Rank::SIX));
    REQUIRE(dealer.getScore() == 17);
    REQUIRE(dealer.getHand().isSoft());
    REQUIRE(dealer.shouldHit() == true);

    // Soft 18 = Ace(11) + 7 = 18 → should stand (18 >= 17 and not soft 17)
    blackjack::Dealer dealer2(blackjack::Difficulty::NORMAL);
    dealer2.receiveCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::ACE));
    dealer2.receiveCard(new blackjack::Card(blackjack::Suit::DIAMONDS, blackjack::Rank::SEVEN));
    REQUIRE(dealer2.getScore() == 18);
    REQUIRE(dealer2.shouldHit() == false);
}

// ---------------------------------------------------------------------------
// DL-02: makeDecision stands on hard 17+.
// ---------------------------------------------------------------------------
TEST_CASE("DL-02: makeDecision stands hard 17+", "[dealer][DL-02]") {
    // Hard 17 = 10 + 7 → stand
    blackjack::Dealer dealer(blackjack::Difficulty::NORMAL);
    dealer.receiveCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::TEN));
    dealer.receiveCard(new blackjack::Card(blackjack::Suit::DIAMONDS, blackjack::Rank::SEVEN));
    REQUIRE(dealer.getScore() == 17);
    REQUIRE(dealer.getHand().isSoft() == false);
    REQUIRE(dealer.shouldHit() == false);

    // Hard 20 = 10 + 10 → stand
    blackjack::Dealer dealer2(blackjack::Difficulty::NORMAL);
    dealer2.receiveCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::TEN));
    dealer2.receiveCard(new blackjack::Card(blackjack::Suit::DIAMONDS, blackjack::Rank::KING));
    REQUIRE(dealer2.getScore() == 20);
    REQUIRE(dealer2.shouldHit() == false);

    // Score 12 → hit (below 17)
    blackjack::Dealer dealer3(blackjack::Difficulty::NORMAL);
    dealer3.receiveCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::SEVEN));
    dealer3.receiveCard(new blackjack::Card(blackjack::Suit::DIAMONDS, blackjack::Rank::FIVE));
    REQUIRE(dealer3.getScore() == 12);
    REQUIRE(dealer3.shouldHit() == true);

    // Score 16 → hit (below 17)
    blackjack::Dealer dealer4(blackjack::Difficulty::NORMAL);
    dealer4.receiveCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::TEN));
    dealer4.receiveCard(new blackjack::Card(blackjack::Suit::DIAMONDS, blackjack::Rank::SIX));
    REQUIRE(dealer4.getScore() == 16);
    REQUIRE(dealer4.shouldHit() == true);
}

// ---------------------------------------------------------------------------
// DL-03: revealHiddenCard — toggles the hole-card flag.
// ---------------------------------------------------------------------------
TEST_CASE("DL-03: revealHiddenCard", "[dealer][DL-03]") {
    blackjack::Dealer dealer(blackjack::Difficulty::NORMAL);
    REQUIRE(dealer.isHoleCardRevealed() == false);
    dealer.revealHiddenCard();
    REQUIRE(dealer.isHoleCardRevealed() == true);
}

// ---------------------------------------------------------------------------
// DL-04: Difficulty field — constructor stores the level correctly.
// ---------------------------------------------------------------------------
TEST_CASE("DL-04: Difficulty field", "[dealer][DL-04]") {
    blackjack::Dealer easy(blackjack::Difficulty::EASY);
    REQUIRE(easy.getLevel() == blackjack::Difficulty::EASY);

    blackjack::Dealer normal(blackjack::Difficulty::NORMAL);
    REQUIRE(normal.getLevel() == blackjack::Difficulty::NORMAL);

    blackjack::Dealer hard(blackjack::Difficulty::HARD);
    REQUIRE(hard.getLevel() == blackjack::Difficulty::HARD);
}

// ---------------------------------------------------------------------------
// DL-05: getScore — returns the blackjack score for the dealer's hand.
// ---------------------------------------------------------------------------
TEST_CASE("DL-05: getScore", "[dealer][DL-05]") {
    // Empty hand
    blackjack::Dealer dealer(blackjack::Difficulty::NORMAL);
    REQUIRE(dealer.getScore() == 0);

    // 10 + 5 = 15
    dealer.receiveCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::TEN));
    dealer.receiveCard(new blackjack::Card(blackjack::Suit::DIAMONDS, blackjack::Rank::FIVE));
    REQUIRE(dealer.getScore() == 15);

    // Natural blackjack: ACE + KING = 21
    blackjack::Dealer dealer2(blackjack::Difficulty::NORMAL);
    dealer2.receiveCard(new blackjack::Card(blackjack::Suit::SPADES, blackjack::Rank::ACE));
    dealer2.receiveCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::KING));
    REQUIRE(dealer2.getScore() == 21);
}

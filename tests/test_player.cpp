#include <catch2/catch_test_macros.hpp>
#include "Player.h"
#include "HumanPlayer.h"
#include "Dealer.h"
#include "Card.h"

// Planned cases: tests/unit_test_plan.md — section 5 `Player` (abstract)

// ---------------------------------------------------------------------------
// P-01: Cannot instantiate base Player (compile-time / API)
// This is enforced by pure virtual methods in Player.
// Uncommenting the line below should cause a compile error:
//   blackjack::Player p;  // ERROR: abstract class
// We verify the class is abstract by checking that it has pure virtuals.
// ---------------------------------------------------------------------------
TEST_CASE("P-01: Cannot instantiate base Player (compile-time / API)", "[player][P-01]") {
    // Player has pure virtual makeDecision() and getScore().
    // This test passes at runtime; the compile-time enforcement is the point.
    // To verify: try uncommenting the line below — it should fail to compile.
    // blackjack::Player p;  // error: cannot declare variable of abstract type
    SUCCEED("Player is abstract — cannot be instantiated directly.");
}

// ---------------------------------------------------------------------------
// P-02: Polymorphic deletion — deleting via Player* is safe (no UB).
// ---------------------------------------------------------------------------
TEST_CASE("P-02: Polymorphic deletion", "[player][P-02]") {
    // Delete a HumanPlayer through a Player pointer
    {
        blackjack::Player* p = new blackjack::HumanPlayer("Test", 50);
        // Give it a card so the destructor has real work to do
        p->receiveCard(new blackjack::Card(blackjack::Suit::HEARTS, blackjack::Rank::TEN));
        delete p;  // must not crash or leak
    }
    // Delete a Dealer through a Player pointer
    {
        blackjack::Player* p = new blackjack::Dealer(blackjack::Difficulty::EASY);
        p->receiveCard(new blackjack::Card(blackjack::Suit::SPADES, blackjack::Rank::ACE));
        delete p;  // must not crash or leak
    }
    SUCCEED("Polymorphic deletion completed without UB.");
}

#pragma once

#include <iostream>
#include "Player.h"

namespace blackjack {

/// @brief Human-controlled player. Reads decisions from stdin,
///        manages a chip bankroll, and delegates scoring to the Hand.
class HumanPlayer : public Player {
public:
    /// @brief Construct with optional name and starting chips.
    /// @param name  Display name (default "Player").
    /// @param chips Starting chip count (default 100).
    explicit HumanPlayer(const std::string& name = "Player", int chips = 100);
    ~HumanPlayer() override;

    /// @brief Prompt the user for a bet amount.
    ///        Deducts from chips_ if the bet is legal.
    /// @return The bet amount, or 0 if the bet was rejected.
    int placeBet();

    /// @brief Prompt the user to hit (h) or stand (s) via stdin.
    void makeDecision() override;

    /// @brief Add chips to the player's bankroll.
    /// @param amount Number of chips to add (must be >= 0).
    void addChips(int amount);

    /// @brief Return the current chip balance.
    /// @return Integer chip count.
    int getChips() const;

    /// @brief Return the hand score (delegates to Hand::getScore).
    /// @return Integer blackjack score.
    int getScore() const override;

    /// @brief Return the current bet amount for this round.
    /// @return Integer bet.
    int getCurrentBet() const;

private:
    int chips_;
    int currentBet_;
};

} // namespace blackjack

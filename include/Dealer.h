#pragma once

#include "Difficulty.h"
#include "Player.h"

namespace blackjack {

/// @brief Automated dealer player. Follows standard house rules:
///        hit on soft 17, stand on hard 17+.
class Dealer : public Player {
public:
    /// @brief Construct with a given difficulty (default NORMAL).
    /// @param level Difficulty that controls house rules.
    explicit Dealer(Difficulty level = Difficulty::NORMAL);
    ~Dealer() override;

    void clearHand() override;

    /// @brief Decide whether to hit or stand based on house rules.
    ///        Hits on soft 17 or below, stands on hard 17+.
    void makeDecision() override;

    /// @brief Check whether the dealer should hit (for use by GameManager).
    /// @return true if the dealer's hand requires another card.
    bool shouldHit() const;

    /// @brief Set the hidden-card-revealed flag to true.
    void revealHiddenCard();

    /// @brief Query whether the hole card has been revealed.
    /// @return true if revealHiddenCard() has been called.
    bool isHoleCardRevealed() const;

    /// @brief Return the hand score (delegates to Hand::getScore).
    /// @return Integer blackjack score.
    int getScore() const override;

    /// @brief Return the dealer's difficulty level.
    /// @return Difficulty enum value.
    Difficulty getLevel() const;

private:
    Difficulty level_;
    bool holeCardRevealed_;
};

} // namespace blackjack

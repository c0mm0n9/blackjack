#include "Dealer.h"
#include "Card.h"

#include <iostream>

namespace blackjack {

/// @brief Construct a Dealer with a given difficulty level.
/// @param level Difficulty that controls house rules (default NORMAL).
Dealer::Dealer(Difficulty level)
    : level_(level), holeCardRevealed_(false) {
    name_ = "Dealer";
}

/// @brief Destructor. Hand cleanup is handled by the base class.
Dealer::~Dealer() = default;

void Dealer::clearHand() {
    holeCardRevealed_ = false;
    Player::clearHand();
}

/// @brief Decide whether to hit or stand based on house rules.
///        Hits on soft 17 or below, stands on hard 17+.
void Dealer::makeDecision() {
    if (shouldHit()) {
        std::cout << "Dealer hits.\n";
    } else {
        std::cout << "Dealer stands.\n";
    }
}

/// @brief Check whether the dealer should take another card.
///        House rule: hit on soft 17 or below, stand on hard 17+.
/// @return true if the dealer's hand requires another card.
bool Dealer::shouldHit() const {
    if (hand_.isBust()) {
        return false;
    }
    int score = hand_.getScore();
    // Stand on hard 17 or above
    if (score >= 17) {
        // If the hand is soft and exactly 17, hit (house rule)
        if (score == 17 && hand_.isSoft()) {
            return true;
        }
        return false;
    }
    // Below 17: always hit
    return true;
}

/// @brief Set the hidden-card-revealed flag to true.
void Dealer::revealHiddenCard() {
    holeCardRevealed_ = true;
}

/// @brief Query whether the hole card has been revealed.
bool Dealer::isHoleCardRevealed() const {
    return holeCardRevealed_;
}

/// @brief Return the hand score (delegates to Hand::getScore).
int Dealer::getScore() const {
    return hand_.getScore();
}

/// @brief Return the dealer's difficulty level.
Difficulty Dealer::getLevel() const {
    return level_;
}

} // namespace blackjack

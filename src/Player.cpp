#include "Player.h"
#include "Card.h"

namespace blackjack {

/// @brief Virtual destructor; ensures proper cleanup via derived classes.
Player::~Player() = default;

/// @brief Deal a card into this player's hand.
/// @param c Pointer to a heap-allocated Card. The Hand takes ownership.
void Player::receiveCard(Card* c) {
    hand_.addCard(c);
}

/// @brief Clear all cards from the player's hand between rounds.
void Player::clearHand() {
    hand_.clearHand();
}

/// @brief Return a read-only reference to the player's hand.
const Hand& Player::getHand() const {
    return hand_;
}

/// @brief Return the player's name.
const std::string& Player::getName() const {
    return name_;
}

} // namespace blackjack

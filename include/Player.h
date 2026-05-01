#pragma once

#include <string>

#include "Hand.h"

namespace blackjack {

/// @brief Abstract base class for all player types (HumanPlayer, Dealer).
/// Holds a name and a Hand; derived classes implement decision logic.
class Player {
public:
    virtual ~Player();

    /// @brief Prompt the player to make a hit/stand decision.
    virtual void makeDecision() = 0;

    /// @brief Return the current blackjack score from the player's hand.
    /// @return Integer score (aces adjusted automatically).
    virtual int getScore() const = 0;

    /// @brief Deal a card into this player's hand.
    /// @param c Pointer to a heap-allocated Card (Hand takes ownership).
    void receiveCard(Card* c);

    /// @brief Clear all cards from the player's hand between rounds.
    void clearHand();

    /// @brief Return a read-only reference to the player's hand.
    /// @return Const reference to the internal Hand object.
    const Hand& getHand() const;

    /// @brief Return the player's name.
    /// @return Const reference to the name string.
    const std::string& getName() const;

protected:
    std::string name_;
    Hand hand_;
};

} // namespace blackjack

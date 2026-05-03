#include "HumanPlayer.h"
#include "Card.h"

#include <iostream>
#include <limits>

namespace blackjack {

/// @brief Construct a HumanPlayer with a name and starting chip count.
/// @param name  Display name (default "Player").
/// @param chips Starting chip balance (default 100).
HumanPlayer::HumanPlayer(const std::string& name, int chips)
    : chips_(chips), currentBet_(0) {
    name_ = name;
}

/// @brief Destructor. Hand cleanup is handled by the base class.
HumanPlayer::~HumanPlayer() = default;

/// @brief Prompt the user to place a bet via stdin.
///        Deducts the bet from chips_ if valid.
/// @return The bet amount if legal; 0 if the bet was rejected.
int HumanPlayer::placeBet() {
    int bet;
    std::cout << "You have " << chips_ << " chips. Enter your bet: ";
    std::cin >> bet;

    // Reject non-positive bets
    if (bet <= 0) {
        std::cout << "Bet must be positive.\n";
        return 0;
    }
    // Reject bets exceeding the player's bankroll
    if (bet > chips_) {
        std::cout << "Insufficient chips. You only have " << chips_ << ".\n";
        return 0;
    }

    chips_ -= bet;
    currentBet_ = bet;
    return bet;
}

/// @brief Prompt the user to hit (h) or stand (s).
///        Reads a single character from stdin. Invalid input is re-prompted.
void HumanPlayer::makeDecision() {
    char choice;
    while (true) {
        std::cout << name_ << ", your score is " << getScore()
                  << ". Hit (h) or Stand (s)? ";
        std::cin >> choice;
        // Clear any remaining input on the line
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 'h' || choice == 'H') {
            std::cout << name_ << " hits.\n";
            break;
        } else if (choice == 's' || choice == 'S') {
            std::cout << name_ << " stands.\n";
            break;
        } else {
            std::cout << "Invalid input. Please enter 'h' or 's'.\n";
        }
    }
}

/// @brief Add chips to the player's bankroll.
/// @param amount Number of chips to add (must be >= 0).
void HumanPlayer::addChips(int amount) {
    if (amount < 0) {
        std::cout << "Cannot add negative chips.\n";
        return;
    }
    chips_ += amount;
}

/// @brief Return the current chip balance.
int HumanPlayer::getChips() const {
    return chips_;
}

/// @brief Return the hand score (delegates to Hand::getScore).
int HumanPlayer::getScore() const {
    return hand_.getScore();
}

/// @brief Return the current bet amount for this round.
int HumanPlayer::getCurrentBet() const {
    return currentBet_;
}

} // namespace blackjack

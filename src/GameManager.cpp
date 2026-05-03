#include "GameManager.h"

#include "Card.h"
#include "Dealer.h"
#include "Deck.h"
#include "Hand.h"
#include "HumanPlayer.h"

#include <iostream>
#include <random>
#include <sstream>

namespace blackjack {

/// @brief Default constructor. Initializes all pointers to nullptr.
GameManager::GameManager()
    : gameDeck_(nullptr), player_(nullptr), dealer_(nullptr),
      currentDifficulty_(Difficulty::NORMAL), forceExit_(false) {}

/// @brief Destructor. Frees owned heap objects.
GameManager::~GameManager() {
    delete gameDeck_;
    delete player_;
    delete dealer_;
}

/// @brief Start a new game session: choose difficulty, create deck/player/dealer, enter game loop.
void GameManager::startGame() {
    std::cout << "===== Blackjack =====\n";

    // For now, use default difficulty Normal
    setDifficulty(Difficulty::NORMAL);

    // Create player with default name
    player_ = new HumanPlayer("Player", 100);

    // Create dealer
    dealer_ = new Dealer(currentDifficulty_);

    // Note: gameLoop() is called separately
}

/// @brief Main game loop. Handles rounds until the player quits or runs out of chips.
void GameManager::gameLoop() {
    bool running = !forceExit_;

    while (running) {
        std::cout << "\n===== New Round =====\n";

        // Reset hands
        player_->clearHand();
        dealer_->clearHand();

        // Place bet
        int bet = player_->placeBet();
        if (bet == 0) {
            std::cout << "No bet placed. Skipping round.\n";
        } else {
            // Deal initial cards (2 each)
            player_->receiveCard(gameDeck_->drawCard());
            dealer_->receiveCard(gameDeck_->drawCard());
            player_->receiveCard(gameDeck_->drawCard());
            dealer_->receiveCard(gameDeck_->drawCard());

            // Show initial hands (dealer's hole card hidden)
            std::cout << "Dealer shows: score " << dealer_->getHand().getScore()
                      << " (hole card hidden)\n";
            std::cout << player_->getName() << " has: score "
                      << player_->getScore() << "\n";

            // Player turn — keep asking until bust or stand
            bool playerDone = false;
            while (!playerDone) {
                if (player_->getHand().isBust()) {
                    std::cout << player_->getName() << " busts!\n";
                    playerDone = true;
                } else {
                    player_->makeDecision();
                    // Check if the player chose to hit (score changed means a card was added)
                    // For simplicity, we just ask and the player re-checks
                    // We use the score to determine — if makeDecision printed "hits",
                    // we need to deal a card. Since makeDecision only prints, we handle
                    // the hit/stand logic here via a helper approach.
                    // Actually, let's use a different approach: read input directly.
                    char choice;
                    std::cout << player_->getName() << ", your score is "
                              << player_->getScore() << ". Hit (h) or Stand (s)? ";
                    std::cin >> choice;

                    if (choice == 'h' || choice == 'H') {
                        player_->receiveCard(gameDeck_->drawCard());
                        std::cout << player_->getName() << " hits. Score: "
                                  << player_->getScore() << "\n";
                    } else {
                        std::cout << player_->getName() << " stands.\n";
                        playerDone = true;
                    }
                }
            }

            // Dealer turn (only if player didn't bust)
            if (!player_->getHand().isBust()) {
                dealer_->revealHiddenCard();
                std::cout << "\nDealer reveals hole card. Score: "
                          << dealer_->getScore() << "\n";

                while (dealer_->shouldHit()) {
                    dealer_->receiveCard(gameDeck_->drawCard());
                    std::cout << "Dealer hits. Score: " << dealer_->getScore() << "\n";
                }
                std::cout << "Dealer stands.\n";
            }

            // Determine result
            std::cout << "\n===== Result =====\n";
            int pScore = player_->getScore();
            int dScore = dealer_->getScore();

            if (player_->getHand().isBust()) {
                std::cout << player_->getName() << " busts! Dealer wins.\n";
                // Chips already deducted by placeBet
            } else if (dealer_->getHand().isBust()) {
                std::cout << "Dealer busts! " << player_->getName() << " wins!\n";
                player_->addChips(bet * 2);
            } else if (pScore > dScore) {
                std::cout << player_->getName() << " wins!\n";
                player_->addChips(bet * 2);
            } else if (pScore < dScore) {
                std::cout << "Dealer wins.\n";
            } else {
                std::cout << "Push. Bet returned.\n";
                player_->addChips(bet);
            }
        }

        std::cout << "Chips: " << player_->getChips() << "\n";

        // Check if player is bankrupt
        if (player_->getChips() <= 0) {
            std::cout << "You're out of chips! Game over.\n";
            running = false;
        } else {
            char c;
            std::cout << "Play again? (y/n): ";
            std::cin >> c;
            if (c != 'y' && c != 'Y') running = false;
        }
    }

    std::cout << "Thanks for playing!\n";
}

/// @brief Save the current game state to a file (placeholder).
void GameManager::saveGame(const std::string& file) {
    std::ofstream out(file);
    if (!out) throw std::runtime_error("Cannot open save file");
    out << "difficulty " << static_cast<int>(currentDifficulty_) << "\n";
    if (player_) out << "player " << player_->getName() << " " << player_->getChips() << "\n";
    out << "dealer " << static_cast<int>(currentDifficulty_) << "\n";
}

/// @brief Load a game state from a file (placeholder).
void GameManager::loadGame(const std::string& file) {
    std::ifstream in(file);
    if (!in) throw std::runtime_error("Cannot open load file");
    std::string line;
    bool hasDifficulty = false, hasPlayer = false, hasDealer = false;
    while (std::getline(in, line)) {
        if (line.find("difficulty ") == 0) {
            int d = std::stoi(line.substr(11));
            setDifficulty(static_cast<Difficulty>(d));
            hasDifficulty = true;
        } else if (line.find("player ") == 0) {
            std::istringstream iss(line.substr(7));
            std::string name;
            int chips;
            iss >> name >> chips;
            player_ = new HumanPlayer(name, chips);
            hasPlayer = true;
        } else if (line.find("dealer ") == 0) {
            int d = std::stoi(line.substr(7));
            dealer_ = new Dealer(static_cast<Difficulty>(d));
            hasDealer = true;
        }
    }
    if (!hasDifficulty || !hasPlayer || !hasDealer) throw std::runtime_error("Invalid save file");
}

/// @brief Set the difficulty and create a new deck with the appropriate number of decks.
/// @param d Difficulty enum value.
void GameManager::setDifficulty(Difficulty d) {
    currentDifficulty_ = d;
    delete gameDeck_;
    int numberOfDecks;
    if (d == Difficulty::EASY) {
        numberOfDecks = 1;
    } else if (d == Difficulty::NORMAL) {
        numberOfDecks = 4;
    } else {
        numberOfDecks = 8;
    }
    gameDeck_ = new Deck(numberOfDecks);
    gameDeck_->shuffle(static_cast<int>(std::random_device{}()));
}

/// @brief Force exit for testing purposes.
/// @param b True to force exit.
void GameManager::forceExitForTest(bool b) {
    forceExit_ = b;
}

/// @brief Return the current game deck pointer.
Deck* GameManager::getGameDeck() const {
    return gameDeck_;
}

} // namespace blackjack

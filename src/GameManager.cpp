#include "GameManager.h"

#include "Dealer.h"
#include "Deck.h"
#include "HumanPlayer.h"

#include <random>

namespace blackjack {

GameManager::GameManager()
    : gameDeck_(nullptr), player_(nullptr), dealer_(nullptr), currentDifficulty_(Difficulty::NORMAL) {}
GameManager::~GameManager() = default;

void GameManager::startGame() {
    std::cout << "===== Blackjack =====\n";

    // Choose difficulty
    int d;
    std::cout << "Select difficulty (0 = Easy, 1 = Normal, 2 = Hard): ";
    std::cin >> d;
    setDifficulty(static_cast<Difficulty>(d));

    // Create deck + shuffle
    gameDeck_ = new Deck();
    gameDeck_->shuffle();

    // Create player
    std::string name;
    std::cout << "Enter your name: ";
    std::cin >> name;
    player_ = new HumanPlayer(name);

    // Create dealer
    dealer_ = new Dealer();

    gameLoop();
}

void GameManager::gameLoop() {
    bool running = true;

    while (running) {
        std::cout << "\n===== New Round =====\n";

        // Reset hands
        player_->clearHand();
        dealer_->clearHand();

        // Deal initial cards
        player_->hit(gameDeck_->draw());
        dealer_->hit(gameDeck_->draw());
        player_->hit(gameDeck_->draw());
        dealer_->hit(gameDeck_->draw());

        // Show table
        dealer_->printHand(true);
        player_->printState();

        // Player turn
        while (!player_->isBusted() && !player_->isStanding()) {
            char action = player_->decideAction(); // 'h' or 's'
            if (action == 'h') {
                player_->hit(gameDeck_->draw());
            } else {
                player_->stand();
            }
        }

        // Dealer turn
        while (dealer_->shouldHit(currentDifficulty_)) {
            dealer_->hit(gameDeck_->draw());
        }

        // Show final
        std::cout << "\n===== Final Hands =====\n";
        dealer_->printHand(false);
        player_->printState();

        // Determine result
        int p = player_->handValue();
        int d = dealer_->handValue();

        if (player_->isBusted()) {
            std::cout << player_->getName() << " busts! Dealer wins.\n";
        } else if (dealer_->isBusted()) {
            std::cout << "Dealer busts! " << player_->getName() << " wins.\n";
        } else if (p > d) {
            std::cout << player_->getName() << " wins!\n";
        } else if (p < d) {
            std::cout << "Dealer wins.\n";
        } else {
            std::cout << "Push.\n";
        }

        // Continue?
        char c;
        std::cout << "\nPlay again? (y/n): ";
        std::cin >> c;
        if (c != 'y' && c != 'Y') running = false;

        // Shuffle if low
        if (gameDeck_->cardsRemaining() < 20) {
            gameDeck_->shuffle();
        }
    }
}

void GameManager::saveGame(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cout << "Failed to save game.\n";
        return;
    }

    // Save difficulty
    file << static_cast<int>(currentDifficulty_) << "\n";

    // Save deck
    file << gameDeck_->serialize() << "\n";

    // Save dealer
    file << dealer_->serialize() << "\n";

    // Save player
    file << player_->serialize() << "\n";

    std::cout << "Game saved to " << filename << "\n";
}

void GameManager::loadGame(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cout << "Save file not found.\n";
        return;
    }

    int diff;
    file >> diff;
    currentDifficulty_ = static_cast<Difficulty>(diff);

    // Load deck
    gameDeck_ = new Deck();
    gameDeck_->deserialize(file);

    // Load dealer
    dealer_ = new Dealer();
    dealer_->deserialize(file);

    // Load player
    player_ = new HumanPlayer("");
    player_->deserialize(file);

    std::cout << "Game loaded from " << filename << "\n";

    gameLoop();
}

void GameManager::setDifficulty(Difficulty d) {
    int numberOfDecks;
    if (d == Difficulty::EASY) {
        numberOfDecks = 1;
    } else if (d == Difficulty::NORMAL) {
        numberOfDecks = 4;
    } else if (d == Difficulty::HARD) {
        numberOfDecks = 8;
    }
    gameDeck_ = new Deck(numberOfDecks);
    gameDeck_->shuffle(std::random_device{}());
}

Deck* GameManager::getGameDeck() const {
    return gameDeck_;
}

} // namespace blackjack

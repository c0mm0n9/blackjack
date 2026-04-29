#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "Difficulty.h" 

namespace blackjack {

class Deck;
class HumanPlayer;
class Dealer;

class GameManager {
public:
    GameManager();
    ~GameManager();

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
};

    deck.shuffle();
    gameLoop();
};
    void gameLoop() {
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
};
    void saveGame() {
    std::ofstream file("savegame.txt");
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

    std::cout << "Game saved.\n";
};
    void loadGame() {
   std::ifstream file("savegame.txt");
    if (!file) {
        std::cout << "No save file found.\n";
        return;
    }

    int diff;
    file >> diff;
    currentDifficulty_ = static_cast<Difficulty>(diff);

    gameDeck_ = new Deck();
    gameDeck_->deserialize(file);

    dealer_ = new Dealer();
    dealer_->deserialize(file);

    player_ = new HumanPlayer("");
    player_->deserialize(file);

    std::cout << "Game loaded.\n";

    gameLoop();
};
    void setDifficulty(Difficulty d) {
        currentDifficulty_ = d;

    switch (d) {
        case Difficulty::Easy:
            std::cout << "Difficulty set to EASY (Dealer hits less).\n";
            break;
        case Difficulty::Normal:
            std::cout << "Difficulty set to NORMAL.\n";
            break;
        case Difficulty::Hard:
            std::cout << "Difficulty set to HARD (Dealer plays optimally).\n";
            break;
    }
};
    Deck* getGameDeck() const;
private:
    Deck* gameDeck_;
    HumanPlayer* player_;
    Dealer* dealer_;
    Difficulty currentDifficulty_;
};

} // namespace blackjack

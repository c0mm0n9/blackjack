#include "GameManager.h"

#include "Dealer.h"
#include "Deck.h"
#include "HumanPlayer.h"

#include <random>

namespace blackjack {

GameManager::GameManager()
    : gameDeck_(nullptr), player_(nullptr), dealer_(nullptr), currentDifficulty_(Difficulty::NORMAL) {}
GameManager::~GameManager() = default;

void GameManager::startGame() {}
void GameManager::gameLoop() {}
void GameManager::saveGame(const std::string&) {}
void GameManager::loadGame(const std::string&) {}
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

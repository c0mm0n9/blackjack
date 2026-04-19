#include "GameManager.h"

#include "Dealer.h"
#include "Deck.h"
#include "HumanPlayer.h"

namespace blackjack {

GameManager::GameManager()
    : gameDeck_(nullptr), player_(nullptr), dealer_(nullptr), currentDifficulty_(Difficulty::NORMAL) {}
GameManager::~GameManager() = default;

void GameManager::startGame() {}
void GameManager::gameLoop() {}
void GameManager::saveGame(const std::string&) {}
void GameManager::loadGame(const std::string&) {}
void GameManager::setDifficulty(Difficulty) {}

} // namespace blackjack

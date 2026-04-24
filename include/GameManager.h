#pragma once

#include <string>

#include "Difficulty.h"

namespace blackjack {

class Deck;
class HumanPlayer;
class Dealer;

class GameManager {
public:
    GameManager();
    ~GameManager();

    void startGame();
    void gameLoop();
    void saveGame(const std::string& filename);
    void loadGame(const std::string& filename);
    void setDifficulty(Difficulty d);
    Deck* getGameDeck() const;
private:
    Deck* gameDeck_;
    HumanPlayer* player_;
    Dealer* dealer_;
    Difficulty currentDifficulty_;
};

} // namespace blackjack

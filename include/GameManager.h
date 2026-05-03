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

  void startGame();
void gameLoop();
void saveGame(const std::string& file);
void loadGame(const std::string& file);
void setDifficulty(Difficulty d);
void forceExitForTest(bool b);

    Deck* getGameDeck() const;
private:
    Deck* gameDeck_;
    HumanPlayer* player_;
    Dealer* dealer_;
    Difficulty currentDifficulty_;
    bool forceExit_;
};

} // namespace blackjack

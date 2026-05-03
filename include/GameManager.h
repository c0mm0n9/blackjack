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
    void startGame(Difficulty difficulty, const std::string& playerName);
    void gameLoop();
    void saveGame();
    void saveGame(const std::string& path);
    /// @return true if a game was loaded; false if cancelled or error.
    bool loadGame();
    void loadGame(const std::string& path);
    void setDifficulty(Difficulty d);

    Difficulty getDifficulty() const;
    Deck* getGameDeck() const;
    HumanPlayer* getPlayer() const;
    Dealer* getDealer() const;

    void forceExitForTest(bool enabled);

private:
    /// Writes current difficulty, name, and chips to last save path or autosave file.
    void persistGameState();
    /// Last file used for save/load; autosave targets this when set.
    std::string lastSavePath_;

    Deck* gameDeck_;
    HumanPlayer* player_;
    Dealer* dealer_;
    Difficulty currentDifficulty_;
    bool forceExitForTest_;
};

} // namespace blackjack

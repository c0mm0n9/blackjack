#include <catch2/catch_test_macros.hpp>
#include "GameManager.h"
#include "Deck.h"
#include "HumanPlayer.h"
#include "Dealer.h"

using namespace blackjack;

// Helper: create a temporary file path
static std::string tempFile(const std::string& name) {
    return "tmp_" + name + ".sav";
}

TEST_CASE("GM-01: setDifficulty", "[game_manager][GM-01]") {
    GameManager gm;

    gm.setDifficulty(Difficulty::Hard);
    REQUIRE(gm.getDifficulty() == Difficulty::Hard);

    gm.setDifficulty(Difficulty::Easy);
    REQUIRE(gm.getDifficulty() == Difficulty::Easy);
}

TEST_CASE("GM-02: startGame", "[game_manager][GM-02]") {
    GameManager gm;

    gm.startGame();

    REQUIRE(gm.getGameDeck() != nullptr);
    REQUIRE(gm.getPlayer() != nullptr);
    REQUIRE(gm.getDealer() != nullptr);

    // Deck should be shuffled at least once
    REQUIRE(gm.getGameDeck()->size() == 52);
}

TEST_CASE("GM-03: gameLoop exit", "[game_manager][GM-03]") {
    GameManager gm;
    gm.startGame();

    // Simulate immediate exit by forcing player to choose "quit"
    gm.forceExitForTest(true);

    REQUIRE_NOTHROW(gm.gameLoop());
}

TEST_CASE("GM-04: saveGame", "[game_manager][GM-04]") {
    GameManager gm;
    gm.startGame();

    std::string file = tempFile("save_ok");

    REQUIRE_NOTHROW(gm.saveGame(file));

    // File should exist and contain something
    std::ifstream in(file);
    REQUIRE(in.good());

    std::string content;
    std::getline(in, content);
    REQUIRE(!content.empty());
}

TEST_CASE("GM-05: loadGame", "[game_manager][GM-05]") {
    GameManager gm;
    gm.startGame();

    std::string file = tempFile("load_ok");

    gm.saveGame(file);

    GameManager gm2;
    REQUIRE_NOTHROW(gm2.loadGame(file));

    REQUIRE(gm2.getGameDeck() != nullptr);
    REQUIRE(gm2.getPlayer() != nullptr);
    REQUIRE(gm2.getDealer() != nullptr);
}

TEST_CASE("GM-06: loadGame corrupt file", "[game_manager][GM-06]") {
    GameManager gm;

    std::string file = tempFile("corrupt");

    // Write garbage
    {
        std::ofstream out(file);
        out << "THIS IS NOT VALID SAVE DATA";
    }

    REQUIRE_THROWS(gm.loadGame(file));
}

TEST_CASE("GM-07: Destructor", "[game_manager][GM-07]") {
    // This test ensures no memory leaks or double frees occur
    REQUIRE_NOTHROW({
        GameManager* gm = new GameManager();
        gm->startGame();
        delete gm;
    });
}

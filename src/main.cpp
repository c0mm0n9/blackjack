#include "GameManager.h"

int main() {
    blackjack::GameManager game;
    game.startGame();
    game.gameLoop();
    return 0;
}

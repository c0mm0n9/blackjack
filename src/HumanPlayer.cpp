#include "HumanPlayer.h"

namespace blackjack {

HumanPlayer::HumanPlayer() : chips_(0) {}
HumanPlayer::~HumanPlayer() = default;

int HumanPlayer::placeBet() { return 0; }
void HumanPlayer::makeDecision() {}
void HumanPlayer::addChips(int) {}
int HumanPlayer::getChips() const { return 0; }
int HumanPlayer::getScore() const { return 0; }

} // namespace blackjack

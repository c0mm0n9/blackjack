#include "Dealer.h"

namespace blackjack {

Dealer::Dealer() : level_(Difficulty::NORMAL) {}
Dealer::~Dealer() = default;

void Dealer::makeDecision() {}
void Dealer::revealHiddenCard() {}
int Dealer::getScore() const { return 0; }

} // namespace blackjack

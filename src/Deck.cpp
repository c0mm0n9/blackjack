#include "Deck.h"

namespace blackjack {

Deck::Deck(int) : cards_(nullptr), topCardIndex_(0) {}
Deck::~Deck() = default;

void Deck::shuffle() {}
Card* Deck::drawCard() { return nullptr; }

} // namespace blackjack

#include "Hand.h"

namespace blackjack {

Hand::Hand() : cards_(nullptr), cardCount_(0) {}
Hand::~Hand() = default;

void Hand::addCard(Card*) {}
int Hand::getScore() const { return 0; }
void Hand::clearHand() {}

} // namespace blackjack

#include "Card.h"

namespace blackjack {

Card::Card(Suit suit, Rank rank)
    : suit_(suit), rank_(rank) {}

Card::~Card() = default;

bool Card::operator==(const Card& other) const {
    return suit_ == other.suit_ && rank_ == other.rank_;
}

}

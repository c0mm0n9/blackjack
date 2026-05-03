#include "Card.h"

#include <string>

namespace blackjack {

namespace {

std::string rankShort(Rank r) {
    switch (r) {
    case Rank::ACE:
        return "A";
    case Rank::TWO:
        return "2";
    case Rank::THREE:
        return "3";
    case Rank::FOUR:
        return "4";
    case Rank::FIVE:
        return "5";
    case Rank::SIX:
        return "6";
    case Rank::SEVEN:
        return "7";
    case Rank::EIGHT:
        return "8";
    case Rank::NINE:
        return "9";
    case Rank::TEN:
        return "10";
    case Rank::JACK:
        return "J";
    case Rank::QUEEN:
        return "Q";
    case Rank::KING:
        return "K";
    }
    return "?";
}

/// UTF-8 suit symbols (source file should be saved as UTF-8; use /utf-8 on MSVC).
const char* suitSymbolUtf8(Suit s) {
    switch (s) {
    case Suit::HEARTS:
        return "\xe2\x99\xa5"; // ♥
    case Suit::DIAMONDS:
        return "\xe2\x99\xa6"; // ♦
    case Suit::CLUBS:
        return "\xe2\x99\xa3"; // ♣
    case Suit::SPADES:
        return "\xe2\x99\xa0"; // ♠
    }
    return "?";
}

} // namespace

Card::Card(Suit suit, Rank rank)
    : suit_(suit), rank_(rank) {}

Card::~Card() = default;

bool Card::operator==(const Card& other) const {
    return suit_ == other.suit_ && rank_ == other.rank_;
}

std::string Card::toDisplayString() const {
    return rankShort(rank_) + suitSymbolUtf8(suit_);
}

} // namespace blackjack

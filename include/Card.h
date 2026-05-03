#pragma once

#include <string>

namespace blackjack {

enum class Suit {
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES,
};
/// Unique values 1–13 so deck order, scoring, and display (10 vs J/Q/K) are consistent.
enum class Rank : int {
    ACE = 1,
    TWO = 2,
    THREE = 3,
    FOUR = 4,
    FIVE = 5,
    SIX = 6,
    SEVEN = 7,
    EIGHT = 8,
    NINE = 9,
    TEN = 10,
    JACK = 11,
    QUEEN = 12,
    KING = 13,
};

class Card {
public:
    Suit suit_;
    Rank rank_;

    Card(Suit suit, Rank rank);
    ~Card();
    bool operator==(const Card& other) const;

    /// Short rank + UTF-8 suit symbol, e.g. "9♥", "10♠".
    std::string toDisplayString() const;
};

} // namespace blackjack

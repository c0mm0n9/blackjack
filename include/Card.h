#pragma once

#include <string>

namespace blackjack {

enum class Suit {
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES,
};
enum class Rank {
    ACE=11,
    TWO=2,
    THREE=3,
    FOUR=4,
    FIVE=5,
    SIX=6,
    SEVEN=7,
    EIGHT=8,
    NINE=9,
    TEN=10,
    JACK=10,
    QUEEN=10,
    KING=10,
};

class Card {
public:
    Suit suit_;
    Rank rank_;

    Card(Suit suit, Rank rank);
    ~Card();
    bool operator==(const Card& other) const;
};

} // namespace blackjack

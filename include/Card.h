#pragma once

#include <string>

namespace blackjack {

class Card {
public:
    std::string suit;
    int rank;

    Card();
    ~Card();
};

} // namespace blackjack

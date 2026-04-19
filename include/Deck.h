#pragma once

namespace blackjack {

class Card;

class Deck {
public:
    explicit Deck(int numberOfDecks);
    ~Deck();

    void shuffle();
    Card* drawCard();

private:
    Card** cards_;
    int topCardIndex_;
};

} // namespace blackjack

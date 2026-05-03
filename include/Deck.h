#pragma once

namespace blackjack {

class Card;

class Deck {
public:
    explicit Deck(int numberOfDecks);
    ~Deck();

    void shuffle(int seed);
    Card* drawCard();
    int getTotalCards() const;
    int getRemainingCards() const;
    int size() const { return getRemainingCards(); }

private:
    Card** cards_;
    int totalCards_;
    int topCardIndex_;
};

} // namespace blackjack

#pragma once

namespace blackjack {

class Card;

class Hand {
public:
    Hand();
    ~Hand();

    void addCard(Card* c);
    int getScore() const;
    void clearHand();

private:
    Card** cards_;
    int cardCount_;
};

} // namespace blackjack

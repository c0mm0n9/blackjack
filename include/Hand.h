#pragma once

namespace blackjack {

class Card;

class Hand {
public:
    Hand();
    ~Hand();

    void addCard(Card* c);
    int getScore() const;
    bool isBust() const;
    bool isNaturalBlackjack() const;
    void clearHand();
    bool isSoft() const;

private:
    Card** cards_;
    int cardCount_;
    int capacity_;
};

} // namespace blackjack

#pragma once

#include <string>

namespace blackjack {

class Card;

class Hand {
public:
    Hand();
    ~Hand();

    void addCard(Card* c);
    int getCardCount() const;
    /// Score using only the first n cards (for dealer up-card display). n is clamped.
    int getScoreFirstNCards(int n) const;
    int getScore() const;
    bool isBust() const;
    bool isNaturalBlackjack() const;
    void clearHand();
    bool isSoft() const;

    /// Comma-separated `Card::toDisplayString()`. If hiddenCardIndex is in range, that
    /// card is shown as "[hidden]" (dealer hole card).
    std::string formatCards(int hiddenCardIndex = -1) const;

private:
    Card** cards_;
    int cardCount_;
    int capacity_;
};

} // namespace blackjack

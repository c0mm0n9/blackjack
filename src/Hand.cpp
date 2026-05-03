#include "Hand.h"
#include "Card.h"

#include <string>

namespace blackjack {

Hand::Hand() : cards_(nullptr), cardCount_(0), capacity_(0) {}
Hand::~Hand() {
    for (int i = 0; i < cardCount_; i++) {
        delete cards_[i];
    }
    delete[] cards_;
}

int Hand::getCardCount() const {
    return cardCount_;
}

void Hand::addCard(Card* card) {
    if (cardCount_ == capacity_) {
        int newCapacity = (capacity_ == 0) ? 4 : capacity_ * 2;
        Card** newCards = new Card*[newCapacity];

        for (int i = 0; i < cardCount_; ++i) {
            newCards[i] = cards_[i];
        }

        delete[] cards_;
        cards_ = newCards;
        capacity_ = newCapacity;
    }

    cards_[cardCount_] = card;
    cardCount_++;
}

int Hand::getScoreFirstNCards(int n) const {
    if (n <= 0 || cardCount_ == 0) {
        return 0;
    }
    int count = n;
    if (count > cardCount_) {
        count = cardCount_;
    }
    int score = 0;
    int aceCount = 0;
    for (int i = 0; i < count; i++) {
        const Rank r = cards_[i]->rank_;
        if (r == Rank::ACE) {
            score += 11;
            aceCount++;
        } else if (r == Rank::TEN || r == Rank::JACK || r == Rank::QUEEN ||
                   r == Rank::KING) {
            score += 10;
        } else {
            score += static_cast<int>(r);
        }
    }
    while (score > 21 && aceCount > 0) {
        score -= 10;
        aceCount--;
    }
    return score;
}

int Hand::getScore() const {
    return getScoreFirstNCards(cardCount_);
}

std::string Hand::formatCards(int hiddenCardIndex) const {
    if (cardCount_ == 0) {
        return "(no cards)";
    }
    std::string out;
    for (int i = 0; i < cardCount_; i++) {
        if (i > 0) {
            out += ", ";
        }
        if (hiddenCardIndex >= 0 && hiddenCardIndex < cardCount_ &&
            i == hiddenCardIndex) {
            out += "[hidden]";
        } else {
            out += cards_[i]->toDisplayString();
        }
    }
    return out;
}

bool Hand::isBust() const {
    return getScore() > 21;
}

bool Hand::isNaturalBlackjack() const {
    // Two cards totaling 21 is always a natural with a standard deck (A + 10/J/Q/K).
    return cardCount_ == 2 && getScore() == 21;
}

void Hand::clearHand() {
    for (int i = 0; i < cardCount_; i++) {
        delete cards_[i];
    }
    cardCount_ = 0;
}

/// @brief Check if the hand is "soft" (at least one ace counted as 11).
/// @return true if at least one ace contributes 11 to the score.
bool Hand::isSoft() const {
    int score = 0;
    int aceCount = 0;
    for (int i = 0; i < cardCount_; i++) {
        const Rank r = cards_[i]->rank_;
        if (r == Rank::ACE) {
            score += 11;
            aceCount++;
        } else if (r == Rank::TEN || r == Rank::JACK || r == Rank::QUEEN ||
                   r == Rank::KING) {
            score += 10;
        } else {
            score += static_cast<int>(r);
        }
    }
    int acesAsEleven = aceCount;
    while (score > 21 && acesAsEleven > 0) {
        score -= 10;
        acesAsEleven--;
    }
    return acesAsEleven > 0;
}

} // namespace blackjack

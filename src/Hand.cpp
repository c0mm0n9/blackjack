#include "Hand.h"
#include "Card.h"

namespace blackjack {

Hand::Hand() : cards_(nullptr), cardCount_(0), capacity_(0) {}
Hand::~Hand() {
    for (int i = 0; i < cardCount_; i++) {
        delete cards_[i];
    }
    delete[] cards_;
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


int Hand::getScore() const {
    int score = 0;
    int aceCount = 0;
    for (int i = 0; i < cardCount_; i++) {
        int rank = static_cast<int>(cards_[i]->rank_);
        if (rank == static_cast<int>(Rank::ACE)) {
            score += 11;
            aceCount++;
        } else if (
            rank == static_cast<int>(Rank::TEN) || 
            rank == static_cast<int>(Rank::JACK) || 
            rank == static_cast<int>(Rank::QUEEN) || 
            rank == static_cast<int>(Rank::KING)) {
            score += 10;
        } else {
            score += rank;
        }
    }
    // Adjust for aces if score > 21
    while (score > 21 && aceCount > 0) {
        score -= 10;
        aceCount--;
    }
    
    return score;
}

bool Hand::isBust() const {
    return getScore() > 21;
}

bool Hand::isNaturalBlackjack() const {
    return getScore() == 21 && cardCount_ == 2;
}

void Hand::clearHand() {
    for (int i = 0; i < cardCount_; i++) {
        delete cards_[i];
    }
    cardCount_ = 0;
}

} // namespace blackjack

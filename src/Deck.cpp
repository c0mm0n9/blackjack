#include "Deck.h"
#include "Card.h"

#include <algorithm>
#include <random>
#include <stdexcept>

namespace blackjack {

Deck::Deck(int numberOfDecks) : cards_(nullptr), totalCards_(0), topCardIndex_(0) {
    if (numberOfDecks < 1) {
        throw std::invalid_argument("numberOfDecks must be greater than 0");
    }

    totalCards_ = numberOfDecks * 52;
    cards_ = new Card*[totalCards_];

    static const Rank kRanks[] = {
        Rank::ACE,   Rank::TWO,   Rank::THREE, Rank::FOUR,  Rank::FIVE,
        Rank::SIX,   Rank::SEVEN, Rank::EIGHT, Rank::NINE,  Rank::TEN,
        Rank::JACK,  Rank::QUEEN, Rank::KING,
    };

    int idx = 0;
    for (int deck = 0; deck < numberOfDecks; ++deck) {
        for (int suit = 0; suit < 4; ++suit) {
            for (Rank r : kRanks) {
                cards_[idx++] = new Card(static_cast<Suit>(suit), r);
            }
        }
    }
}

Deck::~Deck() {
    // Drawn cards [0, topCardIndex_) are owned by Hands (or already freed); do not delete.
    for (int i = topCardIndex_; i < totalCards_; ++i) {
        delete cards_[i];
    }
    delete[] cards_;
}

void Deck::shuffle(int seed) {
    std::mt19937 gen(seed);
    if (topCardIndex_ < totalCards_) {
        std::shuffle(cards_ + topCardIndex_, cards_ + totalCards_, gen);
    }
}

Card* Deck::drawCard() {
    if (topCardIndex_ >= totalCards_) {
        throw std::runtime_error("Deck is empty");
    }
    return cards_[topCardIndex_++];
}

/// @brief Return the total number of cards in the shoe.
int Deck::getTotalCards() const {
    return totalCards_;
}

/// @brief Return the number of cards not yet drawn.
int Deck::getRemainingCards() const {
    return totalCards_ - topCardIndex_;
}

} // namespace blackjack

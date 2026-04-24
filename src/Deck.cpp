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

    int idx = 0;
    for (int deck = 0; deck < numberOfDecks; ++deck) {
        for (int suit = 0; suit < 4; ++suit) {
            for (int rank = 0; rank < 13; ++rank) {
                cards_[idx++] = new Card(static_cast<Suit>(suit), static_cast<Rank>(rank));
            }
        }
    }
}

Deck::~Deck() {
    for (int i = 0; i < totalCards_; ++i) {
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

} // namespace blackjack

#pragma once

#include "Difficulty.h"
#include "Player.h"

namespace blackjack {

class Dealer : public Player {
public:
    Dealer();
    ~Dealer() override;

    void makeDecision() override;
    void revealHiddenCard();
    int getScore() const override;

private:
    Difficulty level_;
};

} // namespace blackjack

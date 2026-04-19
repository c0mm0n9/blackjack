#pragma once

#include "Player.h"

namespace blackjack {

class HumanPlayer : public Player {
public:
    HumanPlayer();
    ~HumanPlayer() override;

    int placeBet();
    void makeDecision() override;
    void addChips(int amount);
    int getChips() const;
    int getScore() const override;

private:
    int chips_;
};

} // namespace blackjack

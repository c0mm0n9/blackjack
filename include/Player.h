#pragma once

#include <string>

#include "Hand.h"

namespace blackjack {

class Player {
public:
    virtual ~Player();

    virtual void makeDecision() = 0;
    virtual int getScore() const = 0;

protected:
    std::string name_;
    Hand hand_;
};

} // namespace blackjack

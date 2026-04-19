# Blackjack

C++ console blackjack game.

Features:
    - Three difficulty levels
    - Basic game flow management
    - Human and dealer player roles
    - Deck and card handling structures
    - Difficulty and game state management

## Class diagram

```mermaid
classDiagram
    %% Core Data Structures
    class Card {
        +string suit
        +int rank
    }

    class Deck {
        -Card** cards Dynamic Array
        -int topCardIndex
        +Deck(int numberOfDecks) Dynamic Memory
        +~Deck() Memory Cleanup
        +shuffle() Random Event
        +drawCard() Card*
    }

    class Hand {
        -Card** cards
        -int cardCount
        +addCard(Card* c) Dynamic Memory
        +getScore() int
        +clearHand()
    }

    %% Player Entities
    class Player {
        <<Abstract>>
        #string name
        #Hand hand
        +makeDecision() virtual
        +getScore() int
    }

    class HumanPlayer {
        -int chips
        +placeBet() int
        +makeDecision() Console Input
        +addChips(int amount)
        +getChips() int
    }

    class Dealer {
        -Difficulty level
        +makeDecision() AI Logic
        +revealHiddenCard()
    }

    %% Game Management & I/O
    class GameManager {
        -Deck* gameDeck Dynamic Memory
        -HumanPlayer* player
        -Dealer* dealer
        -Difficulty currentDifficulty
        +startGame()
        +gameLoop()
        +saveGame(string filename) File I/O
        +loadGame(string filename) File I/O
        +setDifficulty(Difficulty d)
    }

    class Difficulty {
        <<Enumeration>>
        EASY: 1 deck in game, 3:2 blackjack payout
        NORMAL: 4 decks in game, 3:2 blackjack payout
        HARD: 8 decks in game, 6:5 blackjack payout
    }

    %% Relationships
    Deck "1" *-- "many" Card : Contains
    Hand "1" o-- "many" Card : Holds
    Player "1" *-- "1" Hand : Owns
    Player <|-- HumanPlayer : Inherits
    Player <|-- Dealer : Inherits
    GameManager "1" *-- "1" Deck : Manages
    GameManager "1" *-- "1" HumanPlayer : Manages
    GameManager "1" *-- "1" Dealer : Manages
    GameManager ..> Difficulty : Uses
```

## Layout

| Path | Role |
|------|------|
| `include/Difficulty.h` | `Difficulty` enum |
| `include/Card.h` | `Card` |
| `include/Deck.h` | `Deck` |
| `include/Hand.h` | `Hand` |
| `include/Player.h` | abstract `Player` |
| `include/HumanPlayer.h` | `HumanPlayer` |
| `include/Dealer.h` | `Dealer` |
| `include/GameManager.h` | `GameManager` |
| `src/*.cpp` | Game logic |
| `tests/` | Catch2 test shells and [`tests/unit_test_plan.md`](tests/unit_test_plan.md) |

## Build

From the repository root:

```text
cmake -S . -B build
cmake --build build
```

The first configure downloads **Catch2** (used only for tests) unless you pass `-DBJ_BUILD_TESTS=OFF`.

## Unit tests

After a successful build, run all registered tests with **CTest** (recommended):

```text
ctest --test-dir build -C Debug --output-on-failure
```

Catch2 tag filters apply only when you invoke the binary yourself, for example:

```text
build/blackjack_tests "[deck]"
```

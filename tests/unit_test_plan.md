# Blackjack — unit test plan

This document is the **behavioral specification** for automated tests. [Catch2](https://github.com/catchorg/Catch2) shells live in `tests/test_*.cpp`; each `TEST_CASE` title should stay aligned with the **ID** column here (same prefix and number, e.g. `H-04: …`).

**How to read the tables**

| Column | Meaning |
|--------|---------|
| **ID** | Stable identifier; use in tags like `[H-04]` and in failure messages |
| **Intent** | What risk or rule this test guards |
| **Given** | Starting state or fixtures |
| **When** | Action under test |
| **Then** | Observable outcome (values, state, errors, leaks) |

If the API gains helpers (for example `Hand::isBust()`), extend the **Then** column rather than inventing a new ID for the same rule.

---

## Quick reference — suite to source file

| Suite | IDs | Catch2 file | Primary headers |
|-------|-----|-------------|-----------------|
| Difficulty | D-01 … D-03 | `test_difficulty.cpp` | `include/Difficulty.h` |
| Card | C-01 … C-03 | `test_card.cpp` | `include/Card.h` |
| Deck | DK-01 … DK-07 | `test_deck.cpp` | `include/Deck.h` |
| Hand | H-01 … H-09 | `test_hand.cpp` | `include/Hand.h` |
| Player | P-01, P-02 | `test_player.cpp` | `include/Player.h` |
| HumanPlayer | HP-01 … HP-06 | `test_human_player.cpp` | `include/HumanPlayer.h` |
| Dealer | DL-01 … DL-05 | `test_dealer.cpp` | `include/Dealer.h` |
| GameManager | GM-01 … GM-07 | `test_game_manager.cpp` | `include/GameManager.h` |
| Integration | I-01 … I-04 | `test_integration.cpp` | several |
| Non-functional | N-01, N-02 | `test_nonfunctional.cpp` | build / CI |

---

## Table of contents

1. [Difficulty](#1-difficulty)  
2. [Card](#2-card)  
3. [Deck](#3-deck)  
4. [Hand](#4-hand)  
5. [Player (abstract)](#5-player-abstract)  
6. [HumanPlayer](#6-humanplayer)  
7. [Dealer](#7-dealer)  
8. [GameManager](#8-gamemanager)  
9. [Integration](#9-integration--rules-engine)  
10. [Non-functional](#10-non-functional--tooling)  
11. [Traceability](#traceability)

---

## 1. Difficulty

| ID | Intent | Given | When | Then |
|----|--------|-------|------|------|
| D-01 | All game modes exist and are distinct | Build with full enum | Exercise each enumerator (switch, table, or config) | `EASY`, `NORMAL`, `HARD` are all valid and pairwise distinct |
| D-02 | Shoe size follows product rules | Rules layer maps `Difficulty` → deck count | Resolve shoe for each mode | `EASY` → 1 deck, `NORMAL` → 4, `HARD` → 8 (per README / design) |
| D-03 | Natural blackjack payout follows mode | Payout table keyed by `Difficulty` | Resolve payout for a natural | `EASY` / `NORMAL` → 3:2; `HARD` → 6:5 on naturals |

---

## 2. Card

| ID | Intent | Given | When | Then |
|----|--------|-------|------|------|
| C-01 | Card stores rank and suit correctly | Valid rank and suit inputs | Construct `Card` | Members match inputs; defaults documented if using default ctor |
| C-02 | Equality / ordering is consistent | Two cards with same or different rank/suit | Compare (if operators exist) | Matches documented semantics (value equality vs pointer identity) |
| C-03 | Invalid rank is rejected or documented | Rank outside allowed domain | Construct or assign | Error policy holds (exception, sentinel, or explicit “undefined” doc) |

---

## 3. Deck

| ID | Intent | Given | When | Then |
|----|--------|-------|------|------|
| DK-01 | Shoe has correct population | `numberOfDecks` ≥ 1 | Build deck | Card count is `52 × N`; composition matches N standard decks |
| DK-02 | Invalid deck count is handled | `numberOfDecks` invalid if disallowed | Build deck | Throws, error code, or assert per API contract |
| DK-03 | Shuffle permutes order | Fresh deck; injectable RNG if available | `shuffle()` | Order differs from deterministic “unshuffled” baseline, or statistical check if non-deterministic |
| DK-04 | Draw advances shoe | Non-empty deck | Repeated `drawCard()` | Non-null pointers; no duplicate deals until policy allows reshuffle |
| DK-05 | Empty shoe behavior | All cards drawn | `drawCard()` | `nullptr`, exception, or automatic reshuffle per spec |

---

## 4. Hand

| ID | Intent | Given | When | Then |
|----|--------|-------|------|------|
| H-01 | Empty hand baseline | No cards | `getScore()` (and helpers if present) | Score `0`; not bust / not natural blackjack unless API defines otherwise |
| H-02 | Pip cards sum | Cards without aces (or aces counted low) | Add cards, read score | Total equals sum of blackjack values for those ranks |
| H-03 | Face cards count ten | J, Q, and/or K | Add, score | Each face card contributes 10 |
| H-04 | Soft ace | Ace plus low cards so 11 is usable | Score / soft flags | Best total ≤ 21 uses soft ace (11) when allowed by rules |
| H-05 | Multiple aces | Several aces in one hand | Score | At most one ace as 11 where standard rules apply; total matches reference calculator |
| H-06 | Bust | Cards totaling over 21 | Score / `isBust` | Bust is true or score reflects bust policy |
| H-07 | Reset between rounds | Hand holds cards | `clearHand()` | No cards (or zero count); score reset; memory ownership per design |
| H-08 | `addCard` | Valid `Card*` | `addCard` | Count increases; card participates in score |
| H-09 | Ownership on clear / destroy | Hand owns `Card*` objects | `clearHand` or destructor | No double-free; no leak |

---

## 5. Player (abstract)

| ID | Intent | Given | When | Then |
|----|--------|-------|------|------|
| P-01 | Base type is abstract | N/A | Compile-time / API review | Cannot construct concrete `Player` without implementing pure virtuals |
| P-02 | Virtual destruction is safe | `Player*` to `HumanPlayer` or `Dealer` | `delete` via base pointer | Derived destructors run; no UB |

---

## 6. HumanPlayer

| ID | Intent | Given | When | Then |
|----|--------|-------|------|------|
| HP-01 | Starting bankroll | Constructor with `C` chips | `getChips()` | Returns `C` |
| HP-02 | Add funds | Known balance | `addChips(n)` | Balance increases by `n` |
| HP-03 | Legal bet | Chips ≥ bet `B` | `placeBet()` (or equivalent) | Returns `B`; chips decrease by `B` |
| HP-04 | Insufficient funds | Bet larger than chip stack | Place bet | Bet rejected per contract (0, false, exception) |
| HP-05 | Input-driven decisions | Injectable stdin / callback | `makeDecision()` with scripted commands | Action matches input; garbage input handled |
| HP-06 | Score matches hand | Known cards in `Hand` | `getScore()` | Same total as `Hand::getScore()` for that composition |

---

## 7. Dealer

| ID | Intent | Given | When | Then |
|----|--------|-------|------|------|
| DL-01 | Soft 17 rule | Hand is soft 17 | `makeDecision()` | Hit or stand matches configured house rules |
| DL-02 | Hard 17+ | Hard total ≥ 17 | `makeDecision()` | Stands |
| DL-03 | Hole card reveal | Hidden hole card | `revealHiddenCard()` | Downstream score / UI includes hole card |
| DL-04 | Difficulty stored | Construct with a `Difficulty` | Read level | Matches constructor argument |
| DL-05 | Dealer score | Known up + hole cards | `getScore()` | Matches blackjack scoring for that hand |

---

## 8. GameManager

| ID | Intent | Given | When | Then |
|----|--------|-------|------|------|
| GM-01 | Difficulty propagation | Default manager | `setDifficulty(d)` | Internal state and downstream shoe/payout use `d` |
| GM-02 | Session start | Valid dependencies wired | `startGame()` | Table state matches first-round rules (bets, deal order) |
| GM-03 | Loop termination | End condition (quit / bankrupt / win cap) | `gameLoop()` | Returns; resources freed |
| GM-04 | Save round-trip | Mid-game state | `saveGame(path)` | File exists; parse or checksum validates |
| GM-05 | Load restores state | Valid save file | `loadGame(path)` | Chips, hands, shoe state match file |
| GM-06 | Corrupt save | Truncated / wrong magic | `loadGame` | Fails without corrupting heap |
| GM-07 | Shutdown hygiene | Manager owns deck / players | Destroy `GameManager` | No leaks per ownership model |

---

## 9. Integration — rules engine

| ID | Intent | Given | When | Then |
|----|--------|-------|------|------|
| I-01 | Natural vs 21 | Player natural; dealer not natural | Settle round | Player paid 3:2 or 6:5 per `Difficulty`; peek rules if any |
| I-02 | Push | Same non-bust total | Settle | Bet returned |
| I-03 | Insurance | Dealer shows ace | Insurance offer / resolution | Bankroll and side bet follow rules |
| I-04 | Split / double | Legal split or double | Full flow | Hands and bankroll match standard blackjack expectations |

---

## 10. Non-functional — tooling

| ID | Intent | Given | When | Then |
|----|--------|-------|------|------|
| N-01 | Memory and UB hygiene | Full test binary | CI build with ASan/UBSan (or local) | No sanitizer violations |
| N-02 | Reproducible randomness | Tests needing RNG | Fixed seed or mock RNG | Same sequence across runs |

---

## Traceability

| Plan section | Primary headers |
|--------------|-----------------|
| 1. Difficulty | `include/Difficulty.h` |
| 2. Card | `include/Card.h` |
| 3. Deck | `include/Deck.h` |
| 4. Hand | `include/Hand.h` |
| 5–7. Player hierarchy | `include/Player.h`, `HumanPlayer.h`, `Dealer.h` |
| 8. GameManager | `include/GameManager.h` |
| 9–10. Cross-cutting | Multiple translation units, CI scripts |

When you add a new `TEST_CASE`, add a row here first (or in the same PR) so intent stays discoverable in code review.

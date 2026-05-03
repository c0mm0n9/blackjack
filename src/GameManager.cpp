#include "GameManager.h"

#include "Card.h"
#include "Dealer.h"
#include "Deck.h"
#include "Hand.h"
#include "HumanPlayer.h"

#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>

namespace blackjack {

namespace {

constexpr const char* kSaveMagic = "BLACKJACKSAVE_v1";

/// Chips to return on a winning natural: stake plus bonus (3:2 on Easy/Normal, 6:5 on Hard).
int naturalBlackjackReturn(int bet, Difficulty d) {
    if (bet <= 0) {
        return 0;
    }
    if (d == Difficulty::HARD) {
        return bet + (bet * 6) / 5;
    }
    return bet + (bet * 3) / 2;
}

} // namespace

/// @brief Default constructor. Initializes all pointers to nullptr.
GameManager::GameManager()
    : lastSavePath_(), gameDeck_(nullptr), player_(nullptr), dealer_(nullptr),
      currentDifficulty_(Difficulty::NORMAL), forceExitForTest_(false) {}

/// @brief Destructor. Frees owned heap objects.
GameManager::~GameManager() {
    delete gameDeck_;
    delete player_;
    delete dealer_;
}

/// @brief Start a new game session: main menu, then setup and game loop.
void GameManager::startGame() {
    std::cout << "===== Blackjack =====\n";

    for (;;) {
        char choice;
        std::cout << "New Game (n), Load Game (l), or Quit (q)? ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 'q' || choice == 'Q') {
            std::cout << "Thanks for playing!\n";
            return;
        }

        if (choice == 'l' || choice == 'L') {
            if (loadGame()) {
                gameLoop();
                std::cout << "Returning to main menu.\n\n";
            } else {
                std::cout << "You can try Load again, start a New Game, or "
                             "Quit.\n\n";
            }
            continue;
        }

        if (choice != 'n' && choice != 'N') {
            std::cout << "Please enter n, l, or q.\n\n";
            continue;
        }

        int d;
        std::cout << "Select difficulty (0 = Easy, 1 = Normal, 2 = Hard): ";
        std::cin >> d;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (d < 0 || d > 2) {
            std::cout << "Invalid choice, defaulting to Normal.\n";
            d = 1;
        }

        std::string name;
        std::cout << "Enter your name: ";
        std::cin >> name;

        startGame(static_cast<Difficulty>(d), name);
        gameLoop();
        std::cout << "Returning to main menu.\n\n";
    }
}

void GameManager::startGame(Difficulty difficulty, const std::string& playerName) {
    setDifficulty(difficulty);
    delete player_;
    delete dealer_;
    player_ = new HumanPlayer(playerName, 100);
    dealer_ = new Dealer(currentDifficulty_);
}

/// @brief Main game loop. Handles rounds until the player quits or runs out of chips.
void GameManager::gameLoop() {
    if (forceExitForTest_) {
        std::cout << "Thanks for playing!\n";
        return;
    }

    bool running = true;

    while (running) {
        std::cout << "\n===== New Round =====\n";

        // Reset hands
        player_->clearHand();
        dealer_->clearHand();

        // Place bet
        int bet = player_->placeBet();
        if (bet == 0) {
            std::cout << "No bet placed. Skipping round.\n";
        } else {
            // Deal initial cards (2 each)
            player_->receiveCard(gameDeck_->drawCard());
            dealer_->receiveCard(gameDeck_->drawCard());
            player_->receiveCard(gameDeck_->drawCard());
            dealer_->receiveCard(gameDeck_->drawCard());

            // Show initial hands (dealer's hole card hidden)
            {
                const Hand& dh = dealer_->getHand();
                const bool maskHole = !dealer_->isHoleCardRevealed() &&
                                      dh.getCardCount() >= 2;
                const int visScore =
                    maskHole ? dh.getScoreFirstNCards(1) : dh.getScore();
                std::cout << "Dealer shows: " << dh.formatCards(maskHole ? 1 : -1)
                          << " — ";
                if (maskHole) {
                    std::cout << "visible score " << visScore
                              << " (hole card hidden)\n";
                } else {
                    std::cout << "score " << visScore << "\n";
                }
            }
            const Hand& playerHand = player_->getHand();
            const Hand& dealerHand = dealer_->getHand();

            std::cout << player_->getName() << " has: "
                      << playerHand.formatCards() << " — score "
                      << player_->getScore() << "\n";

            const bool playerNatural =
                playerHand.getCardCount() == 2 && playerHand.getScore() == 21;
            const bool dealerNatural =
                dealerHand.getCardCount() == 2 && dealerHand.getScore() == 21;

            if (playerNatural || dealerNatural) {
                dealer_->revealHiddenCard();
                std::cout << "\nDealer reveals hole card: "
                          << dealer_->getHand().formatCards() << " — score "
                          << dealer_->getScore() << "\n";

                std::cout << "\n===== Result =====\n";
                const int pScore = player_->getScore();
                const int dScore = dealer_->getScore();
                std::cout << player_->getName() << ": "
                          << player_->getHand().formatCards() << " (" << pScore
                          << ")\n";
                std::cout << "Dealer: " << dealer_->getHand().formatCards()
                          << " (" << dScore << ")\n";

                if (playerNatural && dealerNatural) {
                    std::cout << "Push — both have natural blackjack.\n";
                    player_->addChips(bet);
                } else if (playerNatural) {
                    const char* odds =
                        (currentDifficulty_ == Difficulty::HARD) ? "6:5" : "3:2";
                    std::cout << "You have blackjack! You win (" << odds
                              << ").\n";
                    player_->addChips(
                        naturalBlackjackReturn(bet, currentDifficulty_));
                } else {
                    std::cout << "Dealer has natural blackjack. You lose.\n";
                }
            } else {
                // Player turn — keep asking until bust or stand
                bool playerDone = false;
                while (!playerDone) {
                    if (player_->getHand().isBust()) {
                        std::cout << player_->getName() << " busts!\n";
                        playerDone = true;
                    } else if (player_->getScore() == 21) {
                        std::cout << player_->getName()
                                  << " has 21 — standing.\n";
                        playerDone = true;
                    } else {
                        char choice;
                        while (true) {
                            std::cout << player_->getName() << ", your hand: "
                                      << player_->getHand().formatCards()
                                      << " — score " << player_->getScore()
                                      << ". Hit (h) or Stand (s)? ";
                            std::cin >> choice;
                            std::cin.ignore(
                                std::numeric_limits<std::streamsize>::max(),
                                '\n');

                            if (choice == 'h' || choice == 'H') {
                                player_->receiveCard(gameDeck_->drawCard());
                                std::cout << player_->getName() << " hits: "
                                          << player_->getHand().formatCards()
                                          << " — score " << player_->getScore()
                                          << "\n";
                                break;
                            }
                            if (choice == 's' || choice == 'S') {
                                std::cout << player_->getName()
                                          << " stands.\n";
                                playerDone = true;
                                break;
                            }
                            std::cout
                                << "Invalid input. Please enter 'h' or 's'.\n";
                        }
                    }
                }

                // Dealer turn (only if player didn't bust)
                if (!player_->getHand().isBust()) {
                    dealer_->revealHiddenCard();
                    std::cout << "\nDealer reveals hole card. Hand: "
                              << dealer_->getHand().formatCards()
                              << " — score " << dealer_->getScore() << "\n";

                    while (dealer_->shouldHit()) {
                        dealer_->receiveCard(gameDeck_->drawCard());
                        std::cout << "Dealer hits: "
                                  << dealer_->getHand().formatCards()
                                  << " — score " << dealer_->getScore()
                                  << "\n";
                    }
                    if (dealer_->getHand().isBust()) {
                        std::cout << "Dealer busts!\n";
                    } else {
                        std::cout << "Dealer stands.\n";
                    }
                }

                // Determine result
                std::cout << "\n===== Result =====\n";
                int pScore = player_->getScore();
                int dScore = dealer_->getScore();
                std::cout << player_->getName() << ": "
                          << player_->getHand().formatCards() << " (" << pScore
                          << ")\n";
                {
                    const Hand& dh = dealer_->getHand();
                    const bool dMask = !dealer_->isHoleCardRevealed() &&
                                       dh.getCardCount() >= 2;
                    const int dShown =
                        dMask ? dh.getScoreFirstNCards(1) : dScore;
                    std::cout << "Dealer: " << dh.formatCards(dMask ? 1 : -1)
                              << " (" << dShown;
                    if (dMask) {
                        std::cout << " visible; hole not shown";
                    }
                    std::cout << ")\n";
                }

                if (player_->getHand().isBust()) {
                    std::cout << player_->getName() << " busts! Dealer wins.\n";
                    // Chips already deducted by placeBet
                } else if (dealer_->getHand().isBust()) {
                    std::cout << "Dealer busts! " << player_->getName()
                              << " wins!\n";
                    player_->addChips(bet * 2);
                } else if (pScore > dScore) {
                    std::cout << player_->getName() << " wins!\n";
                    player_->addChips(bet * 2);
                } else if (pScore < dScore) {
                    std::cout << "Dealer wins.\n";
                } else {
                    std::cout << "Push. Bet returned.\n";
                    player_->addChips(bet);
                }
            }
        }

        std::cout << "Chips: " << player_->getChips() << "\n";

        persistGameState();

        // Check if player is bankrupt
        if (player_->getChips() <= 0) {
            std::cout << "\nYou're out of chips!\n";
            bool resolved = false;
            while (running && !resolved) {
                std::cout << "Start fresh with 100 chips (f), Load saved game "
                             "(l), or Quit (q)? ";
                char c;
                std::cin >> c;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                                '\n');
                if (c == 'f' || c == 'F') {
                    const std::string name = player_->getName();
                    delete player_;
                    player_ = new HumanPlayer(name, 100);
                    dealer_->clearHand();
                    setDifficulty(currentDifficulty_);
                    std::cout << "You have 100 chips. Good luck!\n";
                    persistGameState();
                    resolved = true;
                } else if (c == 'l' || c == 'L') {
                    std::cout << "Enter save file name to load: ";
                    std::string path;
                    std::getline(std::cin, path);
                    if (path.empty()) {
                        std::cout << "Load cancelled.\n";
                        continue;
                    }
                    try {
                        loadGame(path);
                        std::cout << "Welcome back, " << player_->getName()
                                  << "! You have " << player_->getChips()
                                  << " chips.\n";
                        if (player_->getChips() <= 0) {
                            std::cout << "That save has no chips — pick another "
                                         "option.\n";
                        } else {
                            persistGameState();
                            resolved = true;
                        }
                    } catch (const std::exception& e) {
                        std::cout << "Failed to load: " << e.what() << "\n";
                    }
                } else if (c == 'q' || c == 'Q') {
                    running = false;
                    resolved = true;
                } else {
                    std::cout << "Please enter f, l, or q.\n";
                }
            }
        } else {
            char c;
            std::cout << "Play again (y), Save (s), or Quit (q)? ";
            std::cin >> c;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (c == 's' || c == 'S') {
                saveGame();
                // After saving, ask again
                std::cout << "Play again (y), Save (s), or Quit (q)? ";
                std::cin >> c;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                                '\n');
            }
            if (c != 'y' && c != 'Y') {
                running = false;
            }
        }
    }
}

/// @brief Save the current game state by prompting for a filename.
void GameManager::saveGame() {
    if (!player_) {
        std::cout << "No active game to save.\n";
        return;
    }
    std::string path;
    std::cout << "Enter save file name: ";
    std::getline(std::cin, path);
    if (path.empty()) {
        std::cout << "Save cancelled.\n";
        return;
    }
    try {
        saveGame(path);
        std::cout << "Game saved to " << path << "\n";
    } catch (const std::exception& e) {
        std::cout << "Failed to save: " << e.what() << "\n";
    }
}

void GameManager::saveGame(const std::string& path) {
    if (!player_) {
        throw std::runtime_error("Cannot save: no active player");
    }
    std::ofstream out(path);
    if (!out) {
        throw std::runtime_error("Cannot open save path for writing");
    }
    out << kSaveMagic << '\n';
    out << static_cast<int>(currentDifficulty_) << '\n';
    out << player_->getName() << '\n';
    out << player_->getChips() << '\n';
    lastSavePath_ = path;
}

/// @brief Load a game state by prompting for a filename.
bool GameManager::loadGame() {
    std::string path;
    std::cout << "Enter save file name to load: ";
    std::getline(std::cin, path);
    if (path.empty()) {
        std::cout << "Load cancelled.\n";
        return false;
    }
    try {
        loadGame(path);
        std::cout << "Game loaded from " << path << "\n";
        std::cout << "Welcome back, " << player_->getName()
                  << "! You have " << player_->getChips() << " chips.\n";
        return true;
    } catch (const std::exception& e) {
        std::cout << "Failed to load: " << e.what() << "\n";
        return false;
    }
}

void GameManager::loadGame(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("Failed to open save file");
    }
    std::string magic;
    std::getline(in, magic);
    if (magic != kSaveMagic) {
        throw std::runtime_error("Invalid save file");
    }
    int di = -1;
    in >> di;
    if (!in || di < 0 || di > 2) {
        throw std::runtime_error("Invalid difficulty in save file");
    }
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string name;
    std::getline(in, name);
    int chips = -1;
    in >> chips;
    if (!in || name.empty() || chips < 0) {
        throw std::runtime_error("Invalid player data in save file");
    }

    delete player_;
    delete dealer_;
    player_ = nullptr;
    dealer_ = nullptr;

    setDifficulty(static_cast<Difficulty>(di));
    player_ = new HumanPlayer(name, chips);
    dealer_ = new Dealer(currentDifficulty_);
    lastSavePath_ = path;
}

void GameManager::persistGameState() {
    if (!player_ || forceExitForTest_) {
        return;
    }
    const std::string path =
        lastSavePath_.empty() ? "blackjack_autosave.sav" : lastSavePath_;
    try {
        saveGame(path);
    } catch (const std::exception&) {
        // Autosave is best-effort; avoid interrupting play.
    }
}

/// @brief Set the difficulty and create a new deck with the appropriate number of decks.
/// @param d Difficulty enum value.
void GameManager::setDifficulty(Difficulty d) {
    currentDifficulty_ = d;
    delete gameDeck_;
    int numberOfDecks;
    if (d == Difficulty::EASY) {
        numberOfDecks = 1;
    } else if (d == Difficulty::NORMAL) {
        numberOfDecks = 4;
    } else {
        numberOfDecks = 8;
    }
    gameDeck_ = new Deck(numberOfDecks);
    gameDeck_->shuffle(static_cast<int>(std::random_device{}()));
}

/// @brief Return the current game deck pointer.
Deck* GameManager::getGameDeck() const {
    return gameDeck_;
}

Difficulty GameManager::getDifficulty() const {
    return currentDifficulty_;
}

HumanPlayer* GameManager::getPlayer() const {
    return player_;
}

Dealer* GameManager::getDealer() const {
    return dealer_;
}

void GameManager::forceExitForTest(bool enabled) {
    forceExitForTest_ = enabled;
}

} // namespace blackjack

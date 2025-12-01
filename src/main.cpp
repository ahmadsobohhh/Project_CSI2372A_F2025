#include "CardDeck.h"
#include "Game.h"
#include "RubisDeck.h"
#include "Rules.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

std::string trim(const std::string& input) {
    const auto first = input.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    const auto last = input.find_last_not_of(" \t\r\n");
    return input.substr(first, last - first + 1);
}

std::string promptLine(const std::string& prompt, bool allowEmpty = false) {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) {
            throw std::runtime_error("Input stream closed unexpectedly");
        }
        line = trim(line);
        if (!line.empty() || allowEmpty) {
            return line;
        }
        std::cout << "Please enter a value." << std::endl;
    }
}

int promptInt(const std::string& prompt, int min, int max) {
    while (true) {
        std::string line = promptLine(prompt);
        try {
            int value = std::stoi(line);
            if (value < min || value > max) {
                std::cout << "Enter a number between " << min << " and " << max << "." << std::endl;
                continue;
            }
            return value;
        } catch (const std::exception&) {
            std::cout << "Please enter a valid number." << std::endl;
        }
    }
}

DisplayMode chooseDisplayMode() {
    std::cout << "Display Modes:" << std::endl;
    std::cout << "  1) Base board (5x5 grid)" << std::endl;
    std::cout << "  2) Expert display (row of revealed cards)" << std::endl;
    int choice = promptInt("Choose display mode (1-2): ", 1, 2);
    return choice == 1 ? DisplayMode::Base : DisplayMode::Expert;
}

RulesMode chooseRulesMode() {
    std::cout << "Rules Modes:" << std::endl;
    std::cout << "  1) Base rules" << std::endl;
    std::cout << "  2) Expert rules" << std::endl;
    int choice = promptInt("Choose rules mode (1-2): ", 1, 2);
    return choice == 1 ? RulesMode::Base : RulesMode::Expert;
}

int choosePlayerCount() {
    return promptInt("Enter number of players (2-4): ", 2, 4);
}

bool charToLetter(char c, Letter& letter) {
    switch (std::toupper(static_cast<unsigned char>(c))) {
    case 'A': letter = Letter::A; return true;
    case 'B': letter = Letter::B; return true;
    case 'C': letter = Letter::C; return true;
    case 'D': letter = Letter::D; return true;
    case 'E': letter = Letter::E; return true;
    default: return false;
    }
}

bool charToNumber(char c, Number& number) {
    switch (c) {
    case '1': number = Number::One; return true;
    case '2': number = Number::Two; return true;
    case '3': number = Number::Three; return true;
    case '4': number = Number::Four; return true;
    case '5': number = Number::Five; return true;
    default: return false;
    }
}

bool parsePosition(const std::string& token, Position& position) {
    if (token.size() < 2) {
        return false;
    }
    Letter letter;
    Number number;
    if (!charToLetter(token[0], letter)) {
        return false;
    }
    if (!charToNumber(token[1], number)) {
        return false;
    }
    position = Position{letter, number};
    return true;
}

std::string formatPosition(const Position& pos) {
    std::string result;
    result += letter_symbol(pos.letter);
    result += number_symbol(pos.number);
    return result;
}

Side chooseSide(std::vector<Side>& available) {
    while (true) {
        std::cout << "Available sides:" << std::endl;
        for (std::size_t i = 0; i < available.size(); ++i) {
            std::cout << "  " << (i + 1) << ") " << to_string(available[i]) << std::endl;
        }
        std::string prompt = "Select a side (1-" + std::to_string(available.size()) + "): ";
        int selection = promptInt(prompt, 1, static_cast<int>(available.size()));
        Side side = available[selection - 1];
        available.erase(available.begin() + (selection - 1));
        return side;
    }
}

std::vector<Position> frontCardsForSide(Side side) {
    switch (side) {
    case Side::Top:
        return {{Letter::A, Number::Two}, {Letter::A, Number::Three}, {Letter::A, Number::Four}};
    case Side::Bottom:
        return {{Letter::E, Number::Two}, {Letter::E, Number::Three}, {Letter::E, Number::Four}};
    case Side::Left:
        return {{Letter::B, Number::One}, {Letter::C, Number::One}, {Letter::D, Number::One}};
    case Side::Right:
        return {{Letter::B, Number::Five}, {Letter::C, Number::Five}, {Letter::D, Number::Five}};
    }
    return {};
}

Position promptPosition(const Board& board, const Player& player, bool blockActive) {
    while (true) {
        std::string input = promptLine(player.getName() + ", choose a card (e.g., B3): ");
        Position pos;
        if (!parsePosition(input, pos)) {
            std::cout << "Invalid format. Use a letter A-E followed by a number 1-5." << std::endl;
            continue;
        }
        try {
            if (blockActive && board.isBlocked(pos.letter, pos.number)) {
                std::cout << "That card is blocked for this turn. Choose another." << std::endl;
                continue;
            }
            if (board.isFaceUp(pos.letter, pos.number)) {
                std::cout << "Card " << formatPosition(pos) << " is already face up." << std::endl;
                continue;
            }
            return pos;
        } catch (const OutOfRange&) {
            std::cout << "That position cannot be selected." << std::endl;
        }
    }
}

void revealFrontCards(Player& player, Board& board, const std::vector<Position>& positions) {
    std::cout << "\n" << player.getName() << ", peek at the three cards in front of you." << std::endl;
    for (const auto& pos : positions) {
        try {
            board.turnFaceUp(pos.letter, pos.number);
        } catch (const std::exception&) {
            // ignore invalid peek positions
        }
    }
    std::cout << board;
    promptLine("Press ENTER when you are done peeking...", true);
    for (const auto& pos : positions) {
        try {
            board.turnFaceDown(pos.letter, pos.number);
        } catch (const std::exception&) {
        }
    }
    std::cout << std::string(40, '-') << std::endl;
}

std::size_t nextIndex(const std::vector<Player>& players, std::size_t current) {
    if (players.empty()) {
        return 0;
    }
    return (current + 1) % players.size();
}

struct ExpertResult {
    bool extraFlip{false};
    bool skipNext{false};
    bool placedBlock{false};
    Position currentPosition;
};

bool performPenguin(Board& board, const Position& currentPosition) {
    auto faceUp = board.faceUpCards();
    std::vector<Position> choices;
    for (const auto& entry : faceUp) {
        if (entry.first != currentPosition) {
            choices.push_back(entry.first);
        }
    }
    if (choices.empty()) {
        std::cout << "No other face-up cards to flip down." << std::endl;
        return false;
    }
    std::cout << "Penguin ability: optionally turn one face-up card face down." << std::endl;
    while (true) {
        std::cout << "Available face-up cards:";
        for (const auto& pos : choices) {
            std::cout << ' ' << formatPosition(pos);
        }
        std::cout << std::endl;
        std::string input = promptLine("Enter card to flip down (or press ENTER to skip): ", true);
        if (input.empty()) {
            std::cout << "Penguin action skipped." << std::endl;
            return false;
        }
        Position target;
        if (!parsePosition(input, target)) {
            std::cout << "Invalid coordinate." << std::endl;
            continue;
        }
        bool validChoice = false;
        for (const auto& option : choices) {
            if (option == target) {
                validChoice = true;
                break;
            }
        }
        if (!validChoice) {
            std::cout << "That card is not eligible." << std::endl;
            continue;
        }
        try {
            board.turnFaceDown(target.letter, target.number);
            std::cout << "Card " << formatPosition(target) << " turned face down." << std::endl;
            return true;
        } catch (const std::exception&) {
            std::cout << "Unable to flip that card." << std::endl;
        }
    }
}

bool performWalrus(Board& board) {
    std::cout << "Walrus ability: block a face-down card for the next player." << std::endl;
    while (true) {
        std::string input = promptLine("Enter card to block (or press ENTER to skip): ", true);
        if (input.empty()) {
            std::cout << "No card blocked." << std::endl;
            return false;
        }
        Position target;
        if (!parsePosition(input, target)) {
            std::cout << "Invalid coordinate." << std::endl;
            continue;
        }
        try {
            if (board.isFaceUp(target.letter, target.number)) {
                std::cout << "Card is already face up. Choose a face-down card." << std::endl;
                continue;
            }
            board.clearBlocked();
            board.setBlocked(target.letter, target.number, true);
            std::cout << "Blocked " << formatPosition(target) << " for the next player." << std::endl;
            return true;
        } catch (const std::exception&) {
            std::cout << "Cannot block that card." << std::endl;
        }
    }
}

Position performOctopus(Board& board, const Position& origin) {
    auto neighbours = orthogonal_neighbours(origin);
    std::vector<Position> valid;
    for (const auto& pos : neighbours) {
        if (pos.letter == Letter::C && pos.number == Number::Three) {
            continue;
        }
        try {
            board.getCard(pos.letter, pos.number);
            valid.push_back(pos);
        } catch (const std::exception&) {
        }
    }
    if (valid.empty()) {
        std::cout << "No valid adjacent cards for octopus to swap." << std::endl;
        return origin;
    }
    std::cout << "Octopus ability: swap with an adjacent card." << std::endl;
    while (true) {
        std::cout << "Adjacent options:";
        for (const auto& pos : valid) {
            std::cout << ' ' << formatPosition(pos);
        }
        std::cout << std::endl;
        std::string input = promptLine("Choose card to swap with: ");
        Position target;
        if (!parsePosition(input, target)) {
            std::cout << "Invalid coordinate." << std::endl;
            continue;
        }
        bool isValid = false;
        for (const auto& option : valid) {
            if (option == target) {
                isValid = true;
                break;
            }
        }
        if (!isValid) {
            std::cout << "That card is not adjacent." << std::endl;
            continue;
        }
        board.swapCells(origin, target);
        std::cout << "Swapped " << formatPosition(origin) << " with " << formatPosition(target) << "." << std::endl;
        return target;
    }
}

ExpertResult applyExpertRules(const Card* card, const Position& position, Board& board, const Game& game) {
    ExpertResult result;
    result.currentPosition = position;
    if (!card) {
        return result;
    }
    FaceAnimal animal = static_cast<FaceAnimal>(*card);
    switch (animal) {
    case FaceAnimal::Octopus:
        result.currentPosition = performOctopus(board, position);
        break;
    case FaceAnimal::Penguin:
        if (game.getPreviousCard() == nullptr) {
            std::cout << "Penguin ability requires a previous card; no action taken." << std::endl;
        } else {
            performPenguin(board, position);
        }
        break;
    case FaceAnimal::Walrus:
        result.placedBlock = performWalrus(board);
        break;
    case FaceAnimal::Crab:
        std::cout << "Crab ability: flip another card immediately." << std::endl;
        result.extraFlip = true;
        break;
    case FaceAnimal::Turtle:
        std::cout << "Turtle ability: the next player will be skipped." << std::endl;
        result.skipNext = true;
        break;
    }
    return result;
}

void printScores(const std::vector<Player>& players) {
    std::vector<const Player*> ordered;
    for (const auto& player : players) {
        ordered.push_back(&player);
    }
    std::sort(ordered.begin(), ordered.end(), [](const Player* lhs, const Player* rhs) {
        if (lhs->getNRubies() == rhs->getNRubies()) {
            return lhs->getName() < rhs->getName();
        }
        return lhs->getNRubies() < rhs->getNRubies();
    });
    std::cout << "Rubies standings (least to most):" << std::endl;
    for (const Player* player : ordered) {
        std::cout << "  " << player->getName() << ": " << player->getNRubies();
        std::cout << (player->getNRubies() == 1 ? " ruby" : " rubies") << std::endl;
    }
}

void awardRubies(Player& winner, RubisDeck& deck) {
    if (deck.isEmpty()) {
        deck.reset();
        deck.shuffle();
    }
    std::unique_ptr<Rubis> prize(deck.getNext());
    if (!prize) {
        std::cout << "No rubies left to award." << std::endl;
        return;
    }
    winner.addRubis(*prize);
    std::cout << winner.getName() << " receives " << *prize << "!" << std::endl;
}

void revealInitialCards(Game& game) {
    for (auto& player : game.players()) {
        auto positions = frontCardsForSide(player.getSide());
        revealFrontCards(player, game.board(), positions);
    }
}

Player* findRoundWinner(Game& game) {
    Player* candidate = nullptr;
    for (auto& player : game.players()) {
        if (player.isActive()) {
            candidate = &player;
            break;
        }
    }
    return candidate;
}

void announceFinalWinners(const std::vector<Player>& players) {
    int best = 0;
    for (const auto& player : players) {
        best = std::max(best, player.getNRubies());
    }
    std::vector<const Player*> winners;
    for (const auto& player : players) {
        if (player.getNRubies() == best) {
            winners.push_back(&player);
        }
    }
    if (winners.empty()) {
        std::cout << "No winner could be determined." << std::endl;
        return;
    }
    if (winners.size() == 1) {
        std::cout << "Overall winner: " << winners.front()->getName() << " with "
                  << winners.front()->getNRubies() << (winners.front()->getNRubies() == 1 ? " ruby." : " rubies.")
                  << std::endl;
    } else {
        std::cout << "Overall winners (tie):" << std::endl;
        for (const Player* player : winners) {
            std::cout << "  " << player->getName() << " with " << player->getNRubies()
                      << (player->getNRubies() == 1 ? " ruby" : " rubies") << std::endl;
        }
    }
}

void playGame(Game& game, Rules& rules, RubisDeck& rubisDeck) {
    Board& board = game.board();
    std::vector<Player>& players = game.players();
    bool walrusBlockPending = false;
    bool walrusBlockActive = false;
    int skipCount = 0;

    while (!rules.gameOver(game)) {
        std::cout << "\n=== Round " << (game.getRound() + 1) << " ===" << std::endl;
        board.allFacesDown();
        board.clearBlocked();
        walrusBlockPending = false;
        walrusBlockActive = false;
        skipCount = 0;
        game.resetTurnPointers();
        for (auto& player : players) {
            player.setActive(true);
        }
        revealInitialCards(game);

        std::size_t currentIndex = 0;
        while (!rules.roundOver(game)) {
            Player& currentPlayer = players.at(currentIndex);
            if (!currentPlayer.isActive()) {
                currentIndex = nextIndex(players, currentIndex);
                continue;
            }

            if (skipCount > 0) {
                std::cout << currentPlayer.getName() << " is skipped due to the turtle effect." << std::endl;
                --skipCount;
                currentIndex = nextIndex(players, currentIndex);
                continue;
            }

            if (!board.hasFaceDownCards()) {
                std::cout << currentPlayer.getName() << " has no cards to flip and is eliminated." << std::endl;
                currentPlayer.setActive(false);
                currentIndex = nextIndex(players, currentIndex);
                continue;
            }

            if (walrusBlockPending) {
                walrusBlockActive = true;
                walrusBlockPending = false;
                std::cout << currentPlayer.getName() << " must avoid the blocked card." << std::endl;
            }

            bool mustFlipAgain = false;
            do {
                mustFlipAgain = false;
                Position choice = promptPosition(board, currentPlayer, walrusBlockActive);
                try {
                    if (!board.turnFaceUp(choice.letter, choice.number)) {
                        std::cout << "Card was already face up. Choose again." << std::endl;
                        continue;
                    }
                } catch (const std::exception& ex) {
                    std::cout << "Unable to flip that card: " << ex.what() << std::endl;
                    continue;
                }

                const Card* card = board.getCard(choice.letter, choice.number);
                game.setCurrentCard(card);

                if (walrusBlockActive) {
                    board.clearBlocked();
                    walrusBlockActive = false;
                }

                std::cout << game;

                bool validSelection = rules.isValid(game);
                if (!validSelection) {
                    std::cout << currentPlayer.getName() << " revealed a mismatch and is out of this round." << std::endl;
                    currentPlayer.setActive(false);
                    break;
                }

                if (game.rulesMode() == RulesMode::Expert) {
                    ExpertResult effects = applyExpertRules(card, choice, board, game);
                    if (effects.extraFlip) {
                        mustFlipAgain = true;
                    }
                    if (effects.skipNext) {
                        ++skipCount;
                    }
                    if (effects.placedBlock) {
                        walrusBlockPending = true;
                    }
                }

            } while (mustFlipAgain && currentPlayer.isActive());

            currentIndex = nextIndex(players, currentIndex);
        }

        Player* winner = findRoundWinner(game);
        if (winner) {
            awardRubies(*winner, rubisDeck);
        } else {
            std::cout << "No active players remained to claim rubies." << std::endl;
        }

        printScores(players);
        game.incrementRound();
    }

    std::cout << "\n=== Final Results ===" << std::endl;
    for (auto& player : players) {
        player.setDisplayMode(true);
        std::cout << player << std::endl;
    }
    printScores(players);
    announceFinalWinners(players);
}

} // namespace

int main() {
    try {
        CardDeck& cardDeck = CardDeck::make_CardDeck();
        cardDeck.reset();
        cardDeck.shuffle();

        GameOptions options;
        options.displayMode = chooseDisplayMode();
        options.rulesMode = chooseRulesMode();

        Game game(cardDeck, options);

        int playerCount = choosePlayerCount();
        std::vector<Side> availableSides{Side::Top, Side::Right, Side::Bottom, Side::Left};
        for (int i = 0; i < playerCount; ++i) {
            std::string name = promptLine("Enter name for player " + std::to_string(i + 1) + ": ");
            Side side = chooseSide(availableSides);
            game.addPlayer(Player(name, side));
        }

        RubisDeck& rubisDeck = RubisDeck::make_RubisDeck();
        rubisDeck.reset();
        rubisDeck.shuffle();

        Rules rules(options.rulesMode == RulesMode::Expert);

        playGame(game, rules, rubisDeck);
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}

// Entry point and orchestration logic for the Memoarrr! console implementation.
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

// Description: Strips ASCII whitespace from both ends of the supplied string.
// Parameters: input (const std::string&) user-provided line.
// Returns: std::string containing the trimmed view.
std::string trim(const std::string& input) {
    const auto first = input.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    const auto last = input.find_last_not_of(" \t\r\n");
    return input.substr(first, last - first + 1);
}

// Description: Displays a prompt and reads a full line from std::cin.
// Parameters: prompt (const std::string&), allowEmpty (bool) to accept blank input.
// Returns: std::string containing the trimmed response.
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

// Description: Requests an integer within [min,max] inclusive, re-prompting on error.
// Parameters: prompt (const std::string&), min/max (int) bounds.
// Returns: int value typed by the user that lies within bounds.
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

// Description: Asks the user to choose between base vs. expert display layout.
// Returns: DisplayMode enumerator matching the choice.
DisplayMode chooseDisplayMode() {
    std::cout << "Display Modes:" << std::endl;
    std::cout << "  1) Base board (5x5 grid)" << std::endl;
    std::cout << "  2) Expert display (row of revealed cards)" << std::endl;
    int choice = promptInt("Choose display mode (1-2): ", 1, 2);
    return choice == 1 ? DisplayMode::Base : DisplayMode::Expert;
}

// Description: Asks the user to choose between base vs. expert ruleset.
// Returns: RulesMode enumerator matching the choice.
RulesMode chooseRulesMode() {
    std::cout << "Rules Modes:" << std::endl;
    std::cout << "  1) Base rules" << std::endl;
    std::cout << "  2) Expert rules" << std::endl;
    int choice = promptInt("Choose rules mode (1-2): ", 1, 2);
    return choice == 1 ? RulesMode::Base : RulesMode::Expert;
}

// Description: Prompts for the number of players taking part.
// Returns: int count between 2 and 4 inclusive.
int choosePlayerCount() {
    return promptInt("Enter number of players (2-4): ", 2, 4);
}

// Description: Converts a letter label (A-E) into the Letter enum.
// Parameters: c (char) raw input, letter (Letter&) output parameter.
// Returns: true when conversion succeeds.
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

// Description: Converts a digit (1-5) into the Number enum.
// Parameters: c (char), number (Number&) output parameter.
// Returns: true when conversion succeeds.
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

// Description: Parses a coordinate string (e.g., B3) into a Position struct.
// Parameters: token (const std::string&), position (Position&) output.
// Returns: true if both components were valid.
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

// Description: Serializes a Position into its letter-number representation.
// Parameters: pos (const Position&).
// Returns: std::string such as "A1".
std::string formatPosition(const Position& pos) {
    std::string result;
    result += letter_symbol(pos.letter);
    result += number_symbol(pos.number);
    return result;
}

// Description: Lets a player pick an available Side from the list.
// Parameters: available (std::vector<Side>&) mutable pool of seat choices.
// Returns: Side selected by the user (removed from availability).
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

// Description: Computes the three positions that sit in front of a given side.
// Parameters: side (Side) seat orientation.
// Returns: vector<Position> describing peek locations.
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

// Description: Prompts the current player for a face-down position, respecting blocks.
// Parameters: board (const Board&), player (const Player&), blockActive (bool) whether walrus applies.
// Returns: Position chosen that is valid to flip.
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

// Description: Temporarily flips the three cards in front of a player's side for peeking.
// Parameters: player (Player&), board (Board&), positions (const std::vector<Position>&).
// Returns: void.
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

// Description: Computes the next index in circular order through the players vector.
// Parameters: players (const std::vector<Player>&), current (std::size_t).
// Returns: std::size_t index that follows current.
std::size_t nextIndex(const std::vector<Player>& players, std::size_t current) {
    if (players.empty()) {
        return 0;
    }
    return (current + 1) % players.size();
}

// Bundles the result of processing expert-rule effects for a reveal.
struct ExpertResult {
    bool extraFlip{false};
    bool skipNext{false};
    bool placedBlock{false};
    Position currentPosition;
};

// Description: Allows the current player to turn a different face-up card face-down.
// Parameters: board (Board&), currentPosition (const Position&).
// Returns: true if a card was flipped down, false if skipped.
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

// Description: Lets the player block a face-down card for the next player.
// Parameters: board (Board&).
// Returns: true if a card was blocked, false if skipped.
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

// Description: Swaps the revealed octopus card with an orthogonal neighbour.
// Parameters: board (Board&), origin (const Position&).
// Returns: Position where the octopus card now resides post-swap.
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

// Description: Applies expert animal abilities to the revealed card.
// Parameters: card (const Card*), position (const Position&), board (Board&), game (const Game&).
// Returns: ExpertResult outlining follow-up actions (extra flips, skips, blocks).
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

// Description: Prints players sorted by ruby count from least to most.
// Parameters: players (const std::vector<Player>&).
// Returns: void.
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

// Description: Draws the next Rubis token and awards it to the winner.
// Parameters: winner (Player&), deck (RubisDeck&).
// Returns: void.
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

// Description: Lets each player peek at their three front cards before a round.
// Parameters: game (Game&).
// Returns: void.
void revealInitialCards(Game& game) {
    for (auto& player : game.players()) {
        auto positions = frontCardsForSide(player.getSide());
        revealFrontCards(player, game.board(), positions);
    }
}

// Description: Retrieves the sole remaining active player in the round, if any.
// Parameters: game (Game&).
// Returns: Player* winner pointer or nullptr when none remain.
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

// Description: Prints the final winner(s) based on the highest ruby totals.
// Parameters: players (const std::vector<Player>&).
// Returns: void.
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

// Description: Runs the full seven-round Memoarrr! match loop.
// Parameters: game (Game&), rules (Rules&), rubisDeck (RubisDeck&).
// Returns: void.
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

// Description: Program entry point that configures decks, rules, players, and starts play.
// Returns: int exit code (0 for success, 1 on fatal error).
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

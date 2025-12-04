#pragma once

#include <algorithm>
#include <memory>
#include <vector>

template <typename C>
class DeckFactory {
public:
    virtual ~DeckFactory() = default;

    // No parameters. Randomizes the ordering of the deck contents using std::random_shuffle.
    void shuffle() {
        std::random_shuffle(m_cards.begin(), m_cards.end());
    }

    // No parameters. Removes and returns ownership of the next element, nullptr when empty.
    C* getNext() {
        if (m_cards.empty()) {
            return nullptr;
        }
        std::unique_ptr<C> next = std::move(m_cards.back());
        m_cards.pop_back();
        return next.release();
    }

    // No parameters. Returns true if no cards/rubies remain to draw.
    bool isEmpty() const {
        return m_cards.empty();
    }

    // No parameters. Returns the current collection size for diagnostics.
    std::size_t size() const {
        return m_cards.size();
    }

protected:
    DeckFactory() = default;

    // Parameters: value (std::unique_ptr<C>). Adds freshly constructed object to storage.
    void push(std::unique_ptr<C> value) {
        m_cards.push_back(std::move(value));
    }

    // No parameters. Clears the container without deleting shared storage (used by reset routines).
    void clear() {
        m_cards.clear();
    }

    std::vector<std::unique_ptr<C>> m_cards;
};

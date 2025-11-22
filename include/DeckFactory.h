#pragma once

#include <algorithm>
#include <memory>
#include <vector>

template <typename C>
class DeckFactory {
public:
    virtual ~DeckFactory() = default;

    void shuffle() {
        std::random_shuffle(m_cards.begin(), m_cards.end());
    }

    C* getNext() {
        if (m_cards.empty()) {
            return nullptr;
        }
        std::unique_ptr<C> next = std::move(m_cards.back());
        m_cards.pop_back();
        return next.release();
    }

    bool isEmpty() const {
        return m_cards.empty();
    }

    std::size_t size() const {
        return m_cards.size();
    }

protected:
    DeckFactory() = default;

    void push(std::unique_ptr<C> value) {
        m_cards.push_back(std::move(value));
    }

    void clear() {
        m_cards.clear();
    }

    std::vector<std::unique_ptr<C>> m_cards;
};

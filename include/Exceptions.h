#pragma once

#include <stdexcept>
#include <string>

class OutOfRange : public std::runtime_error {
public:
    explicit OutOfRange(const std::string& msg) : std::runtime_error(msg) {}
};

class NoMoreCards : public std::runtime_error {
public:
    explicit NoMoreCards(const std::string& msg) : std::runtime_error(msg) {}
};

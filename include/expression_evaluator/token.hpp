#pragma once

#include <string>
#include <variant>

namespace expression_evaluator {

enum class TokenType {
    // Literals
    INTEGER,
    FLOAT,
    TRUE,
    FALSE,

    // Operators
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    POWER,
    UNARY_MINUS,

    // Comparison operators
    EQUAL,
    NOT_EQUAL,
    GREATER,
    LESS,
    GREATER_EQUAL,
    LESS_EQUAL,

    // Logical operators
    AND,
    OR,

    // Parentheses
    LEFT_PAREN,
    RIGHT_PAREN,
};

struct Token {
    TokenType type;
    std::variant<int, double, bool, std::monostate> value;

    explicit Token(TokenType t) : type(t), value(std::monostate{}) {}
    Token(TokenType t, int v) : type(t), value(v) {}
    Token(TokenType t, double v) : type(t), value(v) {}
    Token(TokenType t, bool v) : type(t), value(v) {}

    [[nodiscard]] bool is_operator() const noexcept {
        return type == TokenType::PLUS || type == TokenType::MINUS ||
               type == TokenType::MULTIPLY || type == TokenType::DIVIDE ||
               type == TokenType::POWER || type == TokenType::EQUAL ||
               type == TokenType::NOT_EQUAL || type == TokenType::GREATER ||
               type == TokenType::LESS || type == TokenType::GREATER_EQUAL ||
               type == TokenType::LESS_EQUAL || type == TokenType::AND ||
               type == TokenType::OR || type == TokenType::UNARY_MINUS;
    }

    [[nodiscard]] bool is_literal() const noexcept {
        return type == TokenType::INTEGER || type == TokenType::FLOAT ||
               type == TokenType::TRUE || type == TokenType::FALSE;
    }
};

} // namespace expression_evaluator

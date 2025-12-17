#pragma once

#include <expression_evaluator/structures/queue.hpp>
#include <expression_evaluator/token.hpp>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>
#include <variant>

namespace expression_evaluator::evaluator {
struct Value {
    std::variant<double, bool> data;

    explicit Value(double d) : data(d) {}
    explicit Value(bool b) : data(b) {}
    explicit Value(int i) : data(static_cast<double>(i)) {}

    [[nodiscard]] bool is_number() const noexcept {
        return std::holds_alternative<double>(data);
    }
    [[nodiscard]] bool is_bool() const noexcept {
        return std::holds_alternative<bool>(data);
    }

    [[nodiscard]] double as_number() const { return std::get<double>(data); }
    [[nodiscard]] bool as_bool() const { return std::get<bool>(data); }

    [[nodiscard]] std::string to_string() const {
        if (is_number()) {
            std::ostringstream oss;
            oss << std::setprecision(std::numeric_limits<double>::max_digits10)
                << as_number();
            return oss.str();
        } else
            return as_bool() ? "true" : "false";
    }
};

/// @brief Evaluate a postfix expression represented as a queue of tokens
/// @param postfix_queue Queue containing tokens in postfix order
/// @return The resulting value of the evaluated expression
/// @throws std::runtime_error on tokens representing invalid mathematical
/// expressions
[[nodiscard]] Value
evaluate_expression(structures::Queue<Token> &postfix_queue);
} // namespace expression_evaluator::evaluator

#include <cmath>
#include <expression_evaluator/evaluator.hpp>
#include <expression_evaluator/structures/stack.hpp>
#include <stdexcept>

namespace {
using namespace expression_evaluator;

/// @brief Ensure the operand is a number and return it, or throw an error
/// @throws std::runtime_error if the value is not a number
double require_number(const evaluator::Value &val) {
    if (val.is_number())
        return val.as_number();

    throw std::runtime_error("Type error: Expected number, got boolean '" +
                             val.to_string() + "'");
}

/// @brief Ensure the operand is a boolean and return it, or throw an error
/// @throws std::runtime_error if the value is not a boolean
bool require_bool(const evaluator::Value &val) {
    if (val.is_bool())
        return val.as_bool();

    throw std::runtime_error("Type error: Expected boolean, got number '" +
                             val.to_string() + "'");
}

} // namespace

evaluator::Value expression_evaluator::evaluator::evaluate_expression(
    structures::Queue<Token> &postfix_queue) {
    structures::Stack<Value> value_stack;

    while (!postfix_queue.is_empty()) {
        Token token = postfix_queue.dequeue();

        // Push operands directly onto stack
        if (token.type == TokenType::INTEGER)
            value_stack.push(Value{std::get<int>(token.value)});
        else if (token.type == TokenType::FLOAT)
            value_stack.push(Value{std::get<double>(token.value)});
        else if (token.type == TokenType::TRUE)
            value_stack.push(Value{true});
        else if (token.type == TokenType::FALSE)
            value_stack.push(Value{false});

        // Unary operators
        else if (token.type == TokenType::UNARY_MINUS) {
            if (value_stack.is_empty())
                throw std::runtime_error("Invalid expression: missing operand");

            Value operand = value_stack.pop();
            value_stack.push(Value{-require_number(operand)});
        }
        // Binary operators
        else {
            if (value_stack.size() < 2)
                throw std::runtime_error(
                    "Invalid expression: insufficient operands");

            Value right = value_stack.pop();
            Value left = value_stack.pop();

            switch (token.type) {
            // Arithmetic operators
            case TokenType::PLUS:
                value_stack.push(
                    Value{require_number(left) + require_number(right)});
                break;
            case TokenType::MINUS:
                value_stack.push(
                    Value{require_number(left) - require_number(right)});
                break;
            case TokenType::MULTIPLY:
                value_stack.push(
                    Value{require_number(left) * require_number(right)});
                break;
            case TokenType::DIVIDE: {
                const double right_number = require_number(right);
                if (right_number == 0.0)
                    throw std::runtime_error("Math error: division by zero");

                value_stack.push(Value{require_number(left) / right_number});
                break;
            }
            case TokenType::POWER:
                value_stack.push(Value{
                    std::pow(require_number(left), require_number(right))});
                break;

            // Comparison operators
            case TokenType::EQUAL:
                if (left.is_number() && right.is_number())
                    value_stack.push(
                        Value{require_number(left) == require_number(right)});
                else if (left.is_bool() && right.is_bool())
                    value_stack.push(
                        Value{require_bool(left) == require_bool(right)});
                else
                    throw std::runtime_error(
                        "Type error: type mismatch in comparison");

                break;
            case TokenType::NOT_EQUAL:
                if (left.is_number() && right.is_number())
                    value_stack.push(
                        Value{require_number(left) != require_number(right)});
                else if (left.is_bool() && right.is_bool())
                    value_stack.push(
                        Value{require_bool(left) != require_bool(right)});
                else
                    throw std::runtime_error(
                        "Type error: type mismatch in comparison");

                break;
            case TokenType::GREATER:
                value_stack.push(
                    Value{require_number(left) > require_number(right)});
                break;
            case TokenType::LESS:
                value_stack.push(
                    Value{require_number(left) < require_number(right)});
                break;
            case TokenType::GREATER_EQUAL:
                value_stack.push(
                    Value{require_number(left) >= require_number(right)});
                break;
            case TokenType::LESS_EQUAL:
                value_stack.push(
                    Value{require_number(left) <= require_number(right)});
                break;

            // Logical operators
            case TokenType::AND:
                value_stack.push(
                    Value{require_bool(left) && require_bool(right)});
                break;
            case TokenType::OR:
                value_stack.push(
                    Value{require_bool(left) || require_bool(right)});
                break;

            default:
                throw std::runtime_error("Unknown operator");
            }
        }
    }

    if (value_stack.size() != 1)
        throw std::runtime_error("Syntax error: too many operands");

    return value_stack.pop();
}
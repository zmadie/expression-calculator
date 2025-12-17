#include <cctype>
#include <expression_evaluator/lexer.hpp>
#include <stdexcept>
#include <string>

namespace {
bool is_digit(char c) { return std::isdigit(static_cast<unsigned char>(c)); }

bool is_alpha(char c) { return std::isalpha(static_cast<unsigned char>(c)); }
} // namespace

void expression_evaluator::lexer::tokenize(
    std::string_view expression, structures::Queue<Token> &output_queue) {
    size_t current_position = 0;
    // For detecting unary minus
    bool last_was_operator_or_lparen = true;

    while (current_position < expression.length()) {
        char current = expression[current_position];

        // Skip whitespace
        if (std::isspace(static_cast<unsigned char>(current))) {
            current_position++;
            continue;
        }

        // Numbers (integers and floats)
        if (is_digit(current) ||
            (current == '.' && current_position + 1 < expression.length() &&
             is_digit(expression[current_position + 1]))) {
            size_t start = current_position;
            bool has_dot = false;

            while (current_position < expression.length() &&
                   (is_digit(expression[current_position]) ||
                    expression[current_position] == '.')) {
                if (expression[current_position] == '.') {
                    if (has_dot)
                        throw std::runtime_error(
                            "Invalid number: multiple decimal points");

                    has_dot = true;
                }
                current_position++;
            }

            std::string number_string(
                expression.substr(start, current_position - start));
            if (has_dot)
                output_queue.enqueue(
                    Token{TokenType::FLOAT, std::stod(number_string)});
            else
                output_queue.enqueue(
                    Token{TokenType::INTEGER, std::stoi(number_string)});

            last_was_operator_or_lparen = false;
            continue;
        }

        // Keywords (true, false)
        if (is_alpha(current)) {
            size_t start = current_position;
            while (current_position < expression.length() &&
                   is_alpha(expression[current_position]))
                current_position++;

            std::string keyword(
                expression.substr(start, current_position - start));
            if (keyword == "true") {
                output_queue.enqueue(Token{TokenType::TRUE, true});
                last_was_operator_or_lparen = false;
            } else if (keyword == "false") {
                output_queue.enqueue(Token{TokenType::FALSE, false});
                last_was_operator_or_lparen = false;
            } else
                throw std::runtime_error("Unknown keyword: " + keyword);

            continue;
        }

        // Two-character operators
        if (current_position + 1 < expression.length()) {
            std::string two_char_operator =
                std::string(expression.substr(current_position, 2));

            if (two_char_operator == "==") {
                output_queue.enqueue(Token{TokenType::EQUAL});
                current_position += 2;
                last_was_operator_or_lparen = true;
                continue;
            } else if (two_char_operator == "!=") {
                output_queue.enqueue(Token{TokenType::NOT_EQUAL});
                current_position += 2;
                last_was_operator_or_lparen = true;
                continue;
            } else if (two_char_operator == ">=") {
                output_queue.enqueue(Token{TokenType::GREATER_EQUAL});
                current_position += 2;
                last_was_operator_or_lparen = true;
                continue;
            } else if (two_char_operator == "<=") {
                output_queue.enqueue(Token{TokenType::LESS_EQUAL});
                current_position += 2;
                last_was_operator_or_lparen = true;
                continue;
            } else if (two_char_operator == "&&") {
                output_queue.enqueue(Token{TokenType::AND});
                current_position += 2;
                last_was_operator_or_lparen = true;
                continue;
            } else if (two_char_operator == "||") {
                output_queue.enqueue(Token{TokenType::OR});
                current_position += 2;
                last_was_operator_or_lparen = true;
                continue;
            }
        }

        // Single-character operators and parentheses
        switch (current) {
        case '+':
            output_queue.enqueue(Token{TokenType::PLUS});
            last_was_operator_or_lparen = true;
            break;
        case '-':
            if (last_was_operator_or_lparen)
                output_queue.enqueue(Token{TokenType::UNARY_MINUS});
            else
                output_queue.enqueue(Token{TokenType::MINUS});

            last_was_operator_or_lparen = true;
            break;
        case '*':
            output_queue.enqueue(Token{TokenType::MULTIPLY});
            last_was_operator_or_lparen = true;
            break;
        case '/':
            output_queue.enqueue(Token{TokenType::DIVIDE});
            last_was_operator_or_lparen = true;
            break;
        case '^':
            output_queue.enqueue(Token{TokenType::POWER});
            last_was_operator_or_lparen = true;
            break;
        case '>':
            output_queue.enqueue(Token{TokenType::GREATER});
            last_was_operator_or_lparen = true;
            break;
        case '<':
            output_queue.enqueue(Token{TokenType::LESS});
            last_was_operator_or_lparen = true;
            break;
        case '(':
            output_queue.enqueue(Token{TokenType::LEFT_PAREN});
            last_was_operator_or_lparen = true;
            break;
        case ')':
            output_queue.enqueue(Token{TokenType::RIGHT_PAREN});
            last_was_operator_or_lparen = false;
            break;
        default:
            throw std::runtime_error(std::string("Unexpected character: '") +
                                     current + "'");
        }

        current_position++;
    }
}

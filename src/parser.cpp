#include <expression_evaluator/parser.hpp>
#include <expression_evaluator/structures/stack.hpp>
#include <stdexcept>

namespace {
using expression_evaluator::TokenType;

int get_precedence(TokenType type) {
    switch (type) {
    case TokenType::OR:
        return 1;
    case TokenType::AND:
        return 2;
    case TokenType::EQUAL:
    case TokenType::NOT_EQUAL:
        return 3;
    case TokenType::GREATER:
    case TokenType::LESS:
    case TokenType::GREATER_EQUAL:
    case TokenType::LESS_EQUAL:
        return 4;
    case TokenType::PLUS:
    case TokenType::MINUS:
        return 5;
    case TokenType::MULTIPLY:
    case TokenType::DIVIDE:
        return 6;
    case TokenType::UNARY_MINUS:
        return 7;
    case TokenType::POWER:
        return 8;
    default:
        return 0;
    }
}

bool is_right_associative(TokenType type) {
    return type == TokenType::POWER || type == TokenType::UNARY_MINUS;
}
} // namespace

void expression_evaluator::parser::to_postfix(
    structures::Queue<Token> &infix_queue,
    structures::Queue<Token> &postfix_queue) {

    structures::Stack<Token> operator_stack;

    // Shunting-yard algorithm
    while (!infix_queue.is_empty()) {
        Token current_token = infix_queue.dequeue();

        if (current_token.is_literal())
            postfix_queue.enqueue(current_token);
        else if (current_token.type == TokenType::LEFT_PAREN)
            operator_stack.push(current_token);
        else if (current_token.type == TokenType::RIGHT_PAREN) {
            // If a left parenthesis is not found during popping, then there is
            // a mismatched parenthesis in the input
            bool found_left_paren = false;
            while (!operator_stack.is_empty()) {
                Token top_operator = operator_stack.pop();
                if (top_operator.type == TokenType::LEFT_PAREN) {
                    found_left_paren = true;
                    break;
                }

                postfix_queue.enqueue(top_operator);
            }

            if (!found_left_paren)
                throw std::runtime_error(
                    "Syntax error: mismatched parentheses");
        } else if (current_token.is_operator()) {
            while (!operator_stack.is_empty()) {
                Token top_operator = operator_stack.top();

                // Stop at left parenthesis since it delimits the current
                // subexpression
                if (top_operator.type == TokenType::LEFT_PAREN)
                    break;

                int top_operator_prec = get_precedence(top_operator.type);
                int current_token_precedence =
                    get_precedence(current_token.type);

                // An operator should be popped from the stack to the output
                // queue if it has greater (or equal if left-associative)
                // precedence than the current operator.
                bool should_pop;
                if (is_right_associative(current_token.type))
                    should_pop = top_operator_prec > current_token_precedence;
                else
                    should_pop = top_operator_prec >= current_token_precedence;

                if (should_pop)
                    postfix_queue.enqueue(operator_stack.pop());
                else
                    break;
            }

            operator_stack.push(current_token);
        }
    }

    // Place any remaining operators on the stack into the output queue. If any
    // parentheses still remain in the stack, then there was a mismatched
    // parenthesis in the input
    while (!operator_stack.is_empty()) {
        Token top_operator = operator_stack.pop();
        if (top_operator.type == TokenType::LEFT_PAREN ||
            top_operator.type == TokenType::RIGHT_PAREN) {
            throw std::runtime_error("Syntax error: mismatched parentheses");
        }

        postfix_queue.enqueue(top_operator);
    }
}
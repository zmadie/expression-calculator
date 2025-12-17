#pragma once

#include <expression_evaluator/structures/queue.hpp>
#include <expression_evaluator/token.hpp>

namespace expression_evaluator::parser {
/// @brief Convert an infix expression (in a queue) to postfix notation using
/// the shunting-yard algorithm
/// @param infix_queue Queue containing tokens in infix order
/// @param postfix_queue Queue to store tokens in postfix order
/// @throws std::runtime_error on mismatched parentheses
void to_postfix(structures::Queue<Token> &infix_queue,
                structures::Queue<Token> &postfix_queue);
} // namespace expression_evaluator::parser

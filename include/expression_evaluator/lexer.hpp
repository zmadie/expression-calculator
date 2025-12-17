#pragma once
#include <string_view>

#include <expression_evaluator/structures/queue.hpp>
#include <expression_evaluator/token.hpp>

namespace expression_evaluator::lexer {
/// @brief Tokenize an expression string into a queue of tokens in infix order
/// @param expression The expression string to tokenize
/// @param output_queue Queue to store the resulting tokens
/// @throws std::runtime_error on invalid expressions
void tokenize(std::string_view expression,
              structures::Queue<Token> &output_queue);
} // namespace expression_evaluator::lexer

#include <expression_evaluator/evaluator.hpp>
#include <expression_evaluator/lexer.hpp>
#include <expression_evaluator/parser.hpp>
#include <expression_evaluator/structures/queue.hpp>
#include <expression_evaluator/token.hpp>
#include <iostream>

int main() {
    using expression_evaluator::Token;
    using expression_evaluator::evaluator::Value;
    using expression_evaluator::structures::Queue;
    namespace evaluator = expression_evaluator::evaluator;
    namespace lexer = expression_evaluator::lexer;
    namespace parser = expression_evaluator::parser;

    std::cout << "Expression Evaluator" << std::endl;
    std::cout
        << "Supported operators: +, -, *, /, ^, ==, !=, >, <, >=, <=, &&, ||"
        << std::endl;
    std::cout << "Type 'exit' to quit." << std::endl;

    while (true) {
        std::cout << "> ";
        std::string expression;

        // Handle EOF (Ctrl+D)
        if (!std::getline(std::cin, expression))
            break;

        if (expression.empty())
            continue;
        else if (expression == "exit")
            break;

        try {
            Queue<Token> output_queue;
            lexer::tokenize(expression, output_queue);

            Queue<Token> postfix_queue;
            parser::to_postfix(output_queue, postfix_queue);

            Value result = evaluator::evaluate_expression(postfix_queue);

            std::cout << result.to_string() << std::endl;
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    return 0;
}
#include <expression_evaluator/evaluator.hpp>
#include <expression_evaluator/lexer.hpp>
#include <expression_evaluator/parser.hpp>
#include <expression_evaluator/structures/queue.hpp>
#include <expression_evaluator/token.hpp>

#include <cmath>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <sstream>
#include <string_view>

namespace {
using expression_evaluator::Token;
using expression_evaluator::evaluator::Value;
using expression_evaluator::structures::Queue;
namespace evaluator = expression_evaluator::evaluator;
namespace lexer = expression_evaluator::lexer;
namespace parser = expression_evaluator::parser;

Value eval(std::string_view expression) {
    Queue<Token> infix;
    lexer::tokenize(expression, infix);

    Queue<Token> postfix;
    parser::to_postfix(infix, postfix);

    return evaluator::evaluate_expression(postfix);
}

void expect_number(std::string_view expression, double expected,
                   double eps = 1e-12) {
    const Value result = eval(expression);
    if (!result.is_number()) {
        throw std::runtime_error("Expected number for '" +
                                 std::string(expression) + "', got " +
                                 result.to_string());
    }

    const double actual = result.as_number();
    if (std::fabs(actual - expected) > eps) {
        std::ostringstream oss;
        oss << "Expected " << expected << " for '" << expression << "', got "
            << actual;
        throw std::runtime_error(oss.str());
    }
}

void expect_bool(std::string_view expression, bool expected) {
    const Value result = eval(expression);
    if (!result.is_bool()) {
        throw std::runtime_error("Expected boolean for '" +
                                 std::string(expression) + "', got " +
                                 result.to_string());
    }

    const bool actual = result.as_bool();
    if (actual != expected) {
        throw std::runtime_error(
            "Expected " + std::string(expected ? "true" : "false") + " for '" +
            std::string(expression) + "', got " +
            std::string(actual ? "true" : "false"));
    }
}

template <typename Fn> void expect_throws(std::string_view name, Fn &&fn) {
    try {
        fn();
    } catch (const std::exception &) {
        return;
    }

    throw std::runtime_error("Expected exception: " + std::string(name));
}

} // namespace

int main() {
    try {
        expect_number("1 + 2", 3.0);
        expect_number("5 - 8", -3.0);
        expect_number("2 * 3", 6.0);
        expect_number("8 / 4", 2.0);
        expect_number("2 ^ 3", 8.0);

        expect_number("1 + 2 * 3", 7.0);
        expect_number("(1 + 2) * 3", 9.0);
        expect_number("2 ^ 3 ^ 2", 512.0);

        expect_number("-2^2", -4.0);
        expect_number("-(1 + 2)", -3.0);
        expect_number("1 + -2", -1.0);

        expect_bool("3 == 3", true);
        expect_bool("3 != 3", false);
        expect_bool("3 > 2", true);
        expect_bool("2 < 3", true);
        expect_bool("2 >= 2", true);
        expect_bool("2 <= 1", false);

        expect_bool("true == true", true);
        expect_bool("true != false", true);

        expect_bool("true && false", false);
        expect_bool("true || false", true);
        expect_bool("false || false", false);
        expect_bool("true && true", true);

        expect_throws("type error (1 && true)", []() { eval("1 && true"); });
        expect_throws("type error (true + 1)", []() { eval("true + 1"); });
        expect_throws("type error (true > false)",
                      []() { eval("true > false"); });
        expect_throws("type mismatch (1 == true)", []() { eval("1 == true"); });
        expect_throws("unary minus on bool", []() { eval("-true"); });
        expect_throws("division by zero", []() { eval("1 / 0"); });
        expect_throws("mismatched parentheses", []() { eval("(1 + 2"); });

        return 0;
    } catch (const std::exception &e) {
        std::cerr << "TEST FAILED: " << e.what() << '\n';
        return 1;
    }
}

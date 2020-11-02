#include "second_party_math/ExprtkWrappers.h"

#include "exprtk/exprtk.hpp"

#include <assert.h>
#include <iostream>

namespace second_party_math
{
ExprtkSymbolTable::ExprtkSymbolTable()
{
    symbol_table_ = std::make_unique<exprtk::symbol_table<double>>();
}

ExprtkSymbolTable::ExprtkSymbolTable(const ExprtkSymbolTable& other)
{
    symbol_table_ = std::make_unique<exprtk::symbol_table<double>>();
    *symbol_table_ = *other.symbol_table_;
}

ExprtkSymbolTable::ExprtkSymbolTable(ExprtkSymbolTable&& other) noexcept
{
    symbol_table_.swap(other.symbol_table_);
}

ExprtkSymbolTable::~ExprtkSymbolTable() {}

ExprtkSymbolTable& ExprtkSymbolTable::operator=(const ExprtkSymbolTable& other)
{
    symbol_table_  = std::make_unique<exprtk::symbol_table<double>>();
    *symbol_table_ = *other.symbol_table_;
    return *this;
}

void ExprtkSymbolTable::add_variable(const std::string name, double& var_ref)
{
    this->symbol_table_->add_variable(name, var_ref);
}

void ExprtkSymbolTable::reset()
{
    symbol_table_ = std::make_unique<exprtk::symbol_table<double>>();
}

ExprtkExpression::ExprtkExpression() {}

ExprtkExpression::ExprtkExpression(const std::string& function_string,
                                   const ExprtkSymbolTable& symbol_table)

{
    intialize(function_string, symbol_table);
}

ExprtkExpression::ExprtkExpression(ExprtkExpression&& other) noexcept
{
    expression_.swap(other.expression_);
}

ExprtkExpression::~ExprtkExpression() {}

void ExprtkExpression::intialize(const std::string& function_string,
                                 const ExprtkSymbolTable& symbol_table)
{
    this->expression_ = std::make_unique<exprtk::expression<double>>();
    expression_->register_symbol_table(*symbol_table.symbol_table_);

    exprtk::parser<double> parser;
    assert(parser.compile(function_string, *expression_));
}

double ExprtkExpression::value() const
{
    assert(expression_);
    return expression_->value();
}

std::vector<std::string> get_variables(const std::string& function_string)
{
    std::vector<std::string> variable_list;
    assert(exprtk::collect_variables(function_string, variable_list));
    return variable_list;
}

double parse_math_string(const std::string expr_str)
{
    // Evaluate the expression
    exprtk::expression<double> expr;
    exprtk::parser<double> expr_parser;
    expr_parser.compile(expr_str, expr);
    double result = expr.value();
    return result;
}

bool test_exprtk()
{
    typedef exprtk::symbol_table<double> symbol_table_t;
    typedef exprtk::expression<double> expression_t;
    typedef exprtk::parser<double> parser_t;

    symbol_table_t symbol_table;
    expression_t expression;
    parser_t parser;

    double x = 0;
    double y = 0;
    double z = 0;

    symbol_table.add_variable("x", x);
    symbol_table.add_variable("y", y);

    expression.register_symbol_table(symbol_table);

    std::string expression_string = "x * y + 3";
    assert(parser.compile(expression_string, expression));

    x = 1.0;
    y = 2.0;
    assert(expression.value() == 1. * 2. + 3.);

    x = 3.7;
    assert(expression.value() == 3.7 * 2. + 3.);

    y = -9.0;
    assert(expression.value() == 3.7 * -9. + 3.);

    // 'x * -9 + 3' for x in range of [0,100) in steps of 0.0001
    for (x = 0.0; x < 100.0; x += 0.0001)
    {
        assert(expression.value() == x * -9. + 3.);
    }

    // Test how collect_variables works.
    std::vector<std::string> variable_list;
    assert(exprtk::collect_variables(expression_string, variable_list));
    assert(variable_list.size() == 2);

    std::vector<std::string> variable_list2;
    assert(exprtk::collect_variables("x * y + z", variable_list2));
    assert(variable_list2.size() == 3);

    return true;
}
}
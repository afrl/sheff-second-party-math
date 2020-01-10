#include "second_party_math/ExprtkWrappers.h"

#include "exprtk/exprtk.hpp"

namespace second_party_math
{
    void exprtk_example()
    {
        typedef exprtk::symbol_table<double> symbol_table_t;
        typedef exprtk::expression<double>     expression_t;
        typedef exprtk::parser<double>             parser_t;

        symbol_table_t symbol_table;
        expression_t   expression;
        parser_t       parser;

        double x = 0;
        double y = 0;

        std::string expression_string = "x * y + 3";
        symbol_table.add_variable("x", x);
        symbol_table.add_variable("y", y);

        expression.register_symbol_table(symbol_table);

        parser.compile(expression_string, expression);

        x = 1.0;
        y = 2.0;
        expression.value(); // 1 * 2 + 3

        x = 3.7;
        expression.value(); // 3.7 * 2 + 3

        y = -9.0;
        expression.value(); // 3.7 * -9 + 3

        // 'x * -9 + 3' for x in range of [0,100) in steps of 0.0001
        for (x = 0.0; x < 100.0; x += 0.0001)
        {
            expression.value(); // x * -9 + 3
        }
    }

    ExprtkExpression::ExprtkExpression()
    {
        this->symbol_table_ = std::make_unique<exprtk::symbol_table<double>>();
        this->expression_ = std::make_unique<exprtk::expression<double>>();
    }

    ExprtkExpression::~ExprtkExpression()
    {
    }

    void ExprtkExpression::add_variable(const std::string name, double & var_ref)
    {
        symbol_table_->add_variable(name, var_ref);
    }

    void ExprtkExpression::register_symbol_table()
    {
        expression_->register_symbol_table(*symbol_table_);
    }

    bool ExprtkExpression::set_function(const std::string & function_string)
    {
        exprtk::parser<double> parser;
        return parser.compile(function_string, *expression_);
    }
    double ExprtkExpression::value() const
    {
        return expression_->value();
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
}
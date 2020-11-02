#pragma once

#include <memory>
#include <string>
#include <vector>

/// Forward declare exprtk types so the massive header does not have to appear in this header.
namespace exprtk
{
template<typename T>
class expression;
template<typename T>
class symbol_table;
}

namespace second_party_math
{
/// A wrapper around both an exprtk::symbol_table
class ExprtkSymbolTable
{
  public:
    ExprtkSymbolTable();
    ExprtkSymbolTable(const ExprtkSymbolTable&);
    ExprtkSymbolTable(ExprtkSymbolTable&&) noexcept;
    ~ExprtkSymbolTable();
    ExprtkSymbolTable& operator=(const ExprtkSymbolTable&);

    void add_variable(const std::string name, double& var_ref);
    void reset();

    friend class ExprtkExpression;

  private:
    std::unique_ptr<exprtk::symbol_table<double>> symbol_table_;
};

/// A wrapper around both an exprtk::expression
class ExprtkExpression
{
  public:
    ExprtkExpression();
    ExprtkExpression(const std::string& function_string, const ExprtkSymbolTable& symbol_table);
    ExprtkExpression(ExprtkExpression&&) noexcept;
    ~ExprtkExpression();

    void intialize(const std::string& function_string, const ExprtkSymbolTable& symbol_table);
    double value() const;

  private:
    std::unique_ptr<exprtk::expression<double>> expression_;
};

/// @brief Extracts the list of variables that appear in the given expression's string.
std::vector<std::string> get_variables(const std::string& function_string);

/// Parses a string that represents a scalar function (with no variables) and
/// returns the answer.
double parse_math_string(const std::string expr_str);

/// @brief A simple that tests if exprtk works as expected
bool test_exprtk();
}
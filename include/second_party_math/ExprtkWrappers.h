#pragma once

#include <string>
#include <memory>

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
    /// A wrapper around both an exprtk::expression and exprtk::symbol_table
    class ExprtkExpression
    {
    public:
        ExprtkExpression();
        ~ExprtkExpression();

        void add_variable(const std::string name, double & var_ref);
        void register_symbol_table();
        bool set_function(const std::string & function_string);
        double value() const;

    private:
        std::unique_ptr<exprtk::expression<double>> expression_;
        std::unique_ptr<exprtk::symbol_table<double>> symbol_table_;
    };
}
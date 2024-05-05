#ifndef _PARSER_HH
#define _PARSER_HH

#include "world.hh"
#include "token.hh"
#include "component_handle.hh"

enum class LiteralType { Integer, Float, Identifier };
struct LiteralExpr {
    LiteralType type;
    union {
        int integer;
        float real;
        const char *identifier;
    } as;
};

enum class ExpressionType { Literal, Unary, Binary };
struct ExpressionHandle {
    ExpressionType type;
    ComponentHandle expr;
};

enum class UnaryType { Negate, Positive };
struct UnaryExpr {
    UnaryType type;
    ExpressionHandle operand;
};

enum class BinaryType { Add, Subtract, Multiply, Divide };
struct BinaryExpr {
    BinaryType type;
    ExpressionHandle left;
    ExpressionHandle right;
};

ExpressionHandle parse(World &world, std::vector<Token> tokens);
void print_expression(World &world, ExpressionHandle expr);

#endif // _PARSER_HH


#include <optional>
#include <vector>
#include <functional>
#include <iostream>
#include <string>
#include <cassert>

#include "parser.hh"
#include "token.hh"

struct TokenStack {
    std::vector<Token> data;
    size_t pos;

    TokenStack(std::vector<Token> data):
        data(data),
        pos(0) {}

    constexpr std::optional<Token> peek() {
        return pos < data.size()
            ? std::make_optional(data[pos])
            : std::nullopt;
    }

    constexpr std::optional<Token> pop() {
        return pos < data.size()
            ? std::make_optional(data[pos++])
            : std::nullopt;
    }

    std::optional<Token> pop_if(std::function<bool(Token)> predicate) {
        auto token = peek();
        if (token.has_value() && predicate(token.value())) {
            return pop();
        }

        return std::nullopt;
    }
};

#define TRY_OPT(EXPR) ({ auto _res = EXPR; if (!_res.has_value()) return std::nullopt; _res.value(); })

static std::optional<Token> consume_any_operators(
    TokenStack &tokens,
    std::vector<Token> types
) {
    return tokens.pop_if([&types](Token token) {
        for (auto type: types) {
            if (token.type == type.type
                && token.as.op == type.as.op
            ) {
                return true;
            }
        }

        return false;
    });
}

static std::optional<ExpressionHandle> parse_expression(World &world, TokenStack &tokens);
static std::optional<ExpressionHandle> parse_term(World &world, TokenStack &tokens);
static std::optional<ExpressionHandle> parse_factor(World &world, TokenStack &tokens);
static std::optional<ExpressionHandle> parse_unary(World &world, TokenStack &tokens);
static std::optional<ExpressionHandle> parse_primary(World &world, TokenStack &tokens);

#define WORLD_IMPLEMENTATION
#include "world.hh"

#define COMPONENT_HANDLE_IMPLEMENTATION
#include "component_handle.hh"

ExpressionHandle parse(
    World &world,
    std::vector<Token> tokens
) {
    TokenStack stack(tokens);
    return parse_expression(world, stack).value();
}

void print_expression(
    World &world,
    ExpressionHandle expr
) {
    switch (expr.type) {
    case ExpressionType::Literal: {
        auto lit = expr.expr.get<LiteralExpr>(world).value();
        switch (lit.type) {
        case LiteralType::Integer: std::cout << lit.as.integer; break;
        case LiteralType::Float: std::cout << lit.as.real; break;
        case LiteralType::Identifier: std::cout << lit.as.identifier; break;
        }
    } break;
    case ExpressionType::Unary: {
        auto unary = expr.expr.get<UnaryExpr>(world).value();
        switch (unary.type) {
        case UnaryType::Negate: {
            std::cout << "{";
            std::cout << "{-}";
            print_expression(world, unary.operand);
            std::cout << "}";
        } break;
        case UnaryType::Positive: {
            std::cout << "{";
            std::cout << "{+}";
            print_expression(world, unary.operand);
            std::cout << "}";
        } break;
        }
        print_expression(world, unary.operand);
    } break;
    case ExpressionType::Binary: {
        auto binary = expr.expr.get<BinaryExpr>(world).value();
        std::cout << "{";
        print_expression(world, binary.left);
        switch (binary.type) {
        case BinaryType::Add: std::cout << " + "; break;
        case BinaryType::Subtract: std::cout << " - "; break;
        case BinaryType::Multiply: std::cout << " * "; break;
        case BinaryType::Divide: std::cout << " / "; break;
        }
        print_expression(world, binary.right);
        std::cout << "}";
    } break;
    }
}

static std::optional<ExpressionHandle> parse_expression(
    World &world,
    TokenStack &tokens
) {
    return parse_term(world, tokens);
}

static std::optional<ExpressionHandle> parse_term(
    World &world,
    TokenStack &tokens
) {
    auto left = TRY_OPT(parse_factor(world, tokens));
    auto operators = std::vector<Token> {
        Token::op('+'),
        Token::op('-')
    };

    while (auto op = consume_any_operators(tokens, operators)) {
        auto op_char = op.value().as.op;
        BinaryType expr_type;
        switch (op_char) {
        case '+': expr_type = BinaryType::Add; break;
        case '-': expr_type = BinaryType::Subtract; break;
        default: assert(false);
        }

        auto right = TRY_OPT(parse_factor(world, tokens));
        size_t entity = world.new_entity();
        ComponentHandle expr = world.add_component<BinaryExpr>(entity,
            BinaryExpr { expr_type, left, right }
        );

        left = ExpressionHandle { ExpressionType::Binary, expr };
    }

    return std::make_optional(left);
}

static std::optional<ExpressionHandle> parse_factor(
    World &world,
    TokenStack &tokens
) {
    auto left = TRY_OPT(parse_primary(world, tokens));
    auto operators = std::vector<Token> {
        Token::op('*'),
        Token::op('/')
    };

    while (auto op = consume_any_operators(tokens, operators)) {
        auto op_char = op.value().as.op;
        BinaryType expr_type;
        switch (op_char) {
        case '*': expr_type = BinaryType::Multiply; break;
        case '/': expr_type = BinaryType::Divide; break;
        default: assert(false);
        }

        auto right = TRY_OPT(parse_primary(world, tokens));
        size_t entity = world.new_entity();
        ComponentHandle expr = world.add_component<BinaryExpr>(entity,
            BinaryExpr { expr_type, left, right }
        );

        left = ExpressionHandle { ExpressionType::Binary, expr };
    }

    return std::make_optional(left);
}

static std::optional<ExpressionHandle> parse_unary(
    World &world,
    TokenStack &tokens
) {
    auto operators = std::vector<Token> {
        Token::op('+'),
        Token::op('-')
    };

    if (auto op = consume_any_operators(tokens, operators)) {
        auto op_char = op.value().as.op;
        UnaryType expr_type;
        switch (op_char) {
        case '+': expr_type = UnaryType::Positive; break;
        case '-': expr_type = UnaryType::Negate; break;
        default: assert(false);
        }

        auto operand = TRY_OPT(parse_unary(world, tokens));
        size_t entity = world.new_entity();
        ComponentHandle expr = world.add_component<UnaryExpr>(entity,
            UnaryExpr { expr_type, operand }
        );

        return std::make_optional(ExpressionHandle { ExpressionType::Unary, expr });
    }

    return parse_primary(world, tokens);
}

static std::optional<ExpressionHandle> parse_primary(
    World &world,
    TokenStack &tokens
) {
    auto token = tokens.pop();
    if (!token.has_value()) {
        return std::nullopt;
    }

    switch (token.value().type) {
    case TokenType::Integer: {
        size_t entity = world.new_entity();
        ComponentHandle expr = world.add_component<LiteralExpr>(entity,
            LiteralExpr {
                .type = LiteralType::Integer,
                .as = { .integer = token.value().as.integer }
            }
        );

        return std::make_optional(ExpressionHandle { ExpressionType::Literal, expr });
    }
    
    case TokenType::Float: {
        size_t entity = world.new_entity();
        ComponentHandle expr = world.add_component<LiteralExpr>(entity,
            LiteralExpr {
                .type = LiteralType::Float,
                .as = { .real = token.value().as.real }
            }
        );

        return std::make_optional(ExpressionHandle { ExpressionType::Literal, expr });
    }

    case TokenType::Identifier: {
        size_t entity = world.new_entity();
        ComponentHandle expr = world.add_component<LiteralExpr>(entity,
            LiteralExpr {
                .type = LiteralType::Identifier,
                .as = { .identifier = token.value().as.identifier }
            }
        );

        return std::make_optional(ExpressionHandle { ExpressionType::Literal, expr });
    }

    case TokenType::Grouping: {
        if (token.value().as.grouping == '(') {
            auto expr = TRY_OPT(parse_expression(world, tokens));
            if (!tokens.pop_if([](Token token) { return token.as.grouping == ')'; })) {
                return std::nullopt;
            }

            return std::make_optional(expr);
        }

        return std::nullopt;
    }

    default: return std::nullopt;
    }
}

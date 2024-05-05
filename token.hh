#ifndef _TOKEN_HH
#define _TOKEN_HH

enum class TokenType {
    Grouping,
    Operator,
    Integer,
    Float,
    Identifier
};

struct Token {
    TokenType type;
    union {
        char grouping;
        char op;
        int integer;
        float real;
        const char *identifier;
    } as;

    static Token grouping(char c) {
        return Token {
            .type = TokenType::Grouping,
            .as = { .grouping = c }
        };
    }

    static Token op(char c) {
        return Token {
            .type = TokenType::Operator,
            .as = { .op = c }
        };
    }

    static Token integer(int i) {
        return Token {
            .type = TokenType::Integer,
            .as = { .integer = i }
        };
    }

    static Token real(float f) {
        return Token {
            .type = TokenType::Float,
            .as = { .real = f }
        };
    }

    static Token identifier(const char *id) {
        return Token {
            .type = TokenType::Identifier,
            .as = { .identifier = id }
        };
    }

};

#endif // _TOKEN_HH

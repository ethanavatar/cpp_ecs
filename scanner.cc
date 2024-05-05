
#include <iostream>
#include <string>
#include <vector>
#include <optional>

#include "token.hh"

struct CharStack {
    std::string data;
    size_t pos;

    CharStack(std::string data):
        data(data),
        pos(0) {}

    std::optional<char> peek() {
        return pos < data.size()
            ? std::make_optional(data[pos])
            : std::nullopt;
    }

    std::optional<char> pop() {
        return pos < data.size()
            ? std::make_optional(data[pos++])
            : std::nullopt;
    }
};

static Token scan_one(CharStack &chars);
static Token scan_identifier(char current, CharStack &chars);
static Token scan_number(char current, CharStack &chars);

std::vector<Token> scan(std::string input) {
    std::vector<Token> tokens;
    CharStack chars(input);

    while (chars.peek().has_value()) {
        Token token = scan_one(chars);
        tokens.push_back(token);
    }

    return tokens;
}

static Token scan_one(CharStack &chars) {
    auto c = chars.pop().value();
    switch (c) {
    case 'a' ... 'z':
    case 'A' ... 'Z':
    case '_': return scan_identifier(c, chars);

    case '(':
    case ')': return Token::grouping(c);

    case '+':
    case '-':
    case '*':
    case '/': return Token::op(c);

    case '0' ... '9': return scan_number(c, chars);

    case ' ':
    case '\t':
    case '\r':
    case '\n': return scan_one(chars);

    default: {
        std::cerr << "Unexpected character: " << c << std::endl;
        exit(1);
    }
    }
}

static Token scan_identifier(char current, CharStack &chars) {
    std::string identifier = { current };

    while (chars.peek().has_value()) {
        auto c = chars.peek().value();
        if (isalnum(c) || c == '_') {
            identifier += chars.pop().value();
        } else {
            break;
        }
    }

    return Token::identifier(identifier.c_str());
}

static Token scan_number(char current, CharStack &chars) {
    std::string number = { current };

    while (chars.peek().has_value()) {
        auto c = chars.peek().value();

        if (!isdigit(c) && c != '.' && c != '_') {
            break;
        }

        if (c != '_') {
            number += c;
        }

        chars.pop();
    }

    if (number.find('.') != std::string::npos) {
        return Token::real(std::stof(number));
    }

    return Token::integer(std::stoi(number));
}

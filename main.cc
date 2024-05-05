#include <cstdio>

#include <world.hh>
#include "scanner.hh"
#include "parser.hh"

int main(void) {
    std::string source = "1 + -2 * 3 / -(+4 - 5)";
    std::vector<Token> tokens = scan(source);

    World world;
    ExpressionHandle expr = parse(world, tokens);

    print_expression(world, expr);

    return 0;
}


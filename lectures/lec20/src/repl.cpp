#include "ast.h"
#include "parser.h"
#include "primops.h"

#include <iostream>
#include <sstream>

using namespace islpp;

int main()
{
    auto env = primop::environment;

    for (;;) {
        std::cerr << "> ";
        std::string entry;
        if (!std::getline(std::cin, entry)) break;

        std::istringstream is(entry);
        try {
            auto program = parse_prog(is);
            env = eval_prog(program, env);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << '\n';
        }
    }
}
#include <fstream>
#include "parser.h"
#include "scope.h"
#include <memory>

void load(const std::string &filename) {
    std::ifstream in(filename);
    Parser par(in);
    GCObjectPtr<Object> exp(nullptr);
    while ((exp = par.nextExpression()).getNormalPointer() != nullptr)
        exp->evalute(Scope::global());
}

int main(int argc, char **argv) {
    load("preload.lisp");
    bool repl_mode = true;
    std::unique_ptr<std::ifstream> ptr;
    std::istream *istr;
    if (argc == 2) {
        repl_mode = false;
        ptr.reset(new std::ifstream(argv[1]));
        istr = ptr.get();
    } else if (argc != 1) {
        std::cerr << "WTF SUP\n";
    } else {
        istr = &std::cin;
        repl_mode = true;
    }
    Parser par(*istr);
    GCObjectPtr<Object> exp(nullptr);
    while ((exp = par.nextExpression()) != nullptr) {
        GCObjectPtr<Object> result = exp->evalute(Scope::global());
        if (repl_mode) {
            result->print(std::cout);
            std::cout << std::endl;
        }
    }
    return 0;
}

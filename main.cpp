#include <fstream>
#include "parser.h"
#include "scope.h"
#include <memory>

int main()
{
    //std::ifstream in("test.lisp");
    Parser par(std::cin);
    Object *exp;
    while ((exp = par.nextExpression()) != nullptr) {
        exp->evalute(Scope::global)->print(std::cout);
        std::cout << std::endl;
    }
    return 0;
}

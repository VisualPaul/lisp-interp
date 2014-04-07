#include <fstream>
#include "parser.h"
#include "scope.h"
#include <memory>

int main()
{
    //std::ifstream in("test.lisp");
    std::unique_ptr<Scope> globalScope(new Scope(nullptr));
    Parser par(std::cin);
    Object *exp;
    while ((exp = par.nextExpression()) != nullptr) {
        exp->evalute(globalScope.get())->print(std::cout);
        std::cout << std::endl;
    }
    return 0;
}

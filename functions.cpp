#include "functions.h"
#include "scope.h"

namespace functions {
    const std::string EvalFunction::NAME("eval");
    Object *EvalFunction::call(Arguments &args) {
        if (args.positionArgs() != 1)
            argumentNumberError(args, 1);
        return args.getArg(0)->evalute(Scope::global());
    }


    
    void init(Scope *scope) {
        numeric_init(scope);
        list_init(scope);
        _add(EvalFunction::object, scope);
    }
}

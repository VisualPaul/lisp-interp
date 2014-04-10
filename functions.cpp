#include "functions.h"
#include "scope.h"

namespace functions {
    const std::string EvalFunction::NAME("eval");
    Object *EvalFunction::call(Arguments &args) {
        if (args.positionArgs() != 1)
            argumentNumberError(args, 1);
        return args.getArg(0)->evalute(Scope::global());
    }

    const std::string ConsFunction::NAME("cons");
    Object *ConsFunction::call(Arguments &args) {
        if (args.positionArgs() != 2)
            argumentNumberError(args, 2);
        return new ConsCell(args.getArg(0), args.getArg(1));
    }
    
    void init(Scope *scope) {
        numeric_init(scope);
        _add(EvalFunction::object, scope);
        _add(ConsFunction::object, scope);
    }
}


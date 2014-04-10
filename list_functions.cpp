#include "list_functions.h"
namespace functions {
const std::string ConsFunction::NAME("cons");
    Object *ConsFunction::call(Arguments &args) {
        if (args.positionArgs() != 2)
            argumentNumberError(args, 2);
        return new ConsCell(args.getArg(0), args.getArg(1));
    }
    
    const std::string CarFunction::NAME("car");
    Object *CarFunction::call(Arguments &args) {
        if (args.positionArgs() != 1)
            argumentNumberError(args, 1);
        return castArgument<ConsCell>(args, 0)->car();
    }
    
    const std::string CdrFunction::NAME("cdr");
    Object *CdrFunction::call(Arguments &args) {
        if (args.positionArgs() != 1)
            argumentNumberError(args, 1);
        return castArgument<ConsCell>(args, 0)->cdr();
    }
    void list_init(Scope *scope) {
        _add(ConsFunction::object, scope);
        _add(CarFunction::object, scope);
        _add(CdrFunction::object, scope);
    }
}

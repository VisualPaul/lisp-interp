#include "functions.h"

namespace functions {
    PlusFunction *const PlusFunction::object = new PlusFunction;
    const std::stirng PlusFunction::NAME("+");
    Object *PlusFunction::call(Arguments &args) {
        bool resultInteger = true;
        for (int i = 0; i < args.positionArgs(); ++i) {
            if (typeid(*args.getArg(i)) != typeid(Integer)) {
                resultInteger = false;
                break;
            }
        }
        if (resultInteger) {
            // We are already sure that all arguments types are Integer so no checks will be performed
            mpz_class result(0);
            for (int i = 0; i < args.positionArgs(); ++i)
                result += static_cast<Integer *>(args.getArg(i))->getVal();
            return new Integer(result);
        } else {
            // we shall check that all of the arguments are either Integer or Double
            double result = 0.0;
            for (int i = 0; i < args.positionArgs(); ++i) {
                Number *arg = castArgument<Number>(args, i);
                result = arg->toDouble();
            }
            return new Double(result);
        }
    }
    std::string PlusFunction::getName() {
        return NAME;
    }
    MinusFunction *const MinusFunction::object = new MinusFunction;
    const std::sting MinusFunction::NAME("-");
    std::string MinusFunction::getName() {
        return NAME;
    }
    Object *MinusFunction::call(Arguments &args) {
        if (args.positionArgs() == 0) {
            
        }
    }
}

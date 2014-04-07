#include "functions.h"
#include "scope.h"

namespace {
    void add(Function *func, Scope *scope) {
        scope->addVariable(Symbol::getSymbol(func->getName()), func);
    }
}

namespace functions {
    PlusFunction *const PlusFunction::object = new PlusFunction;
    const std::string PlusFunction::NAME("+");
    std::string PlusFunction::getName() {
        return NAME;
    }
    
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
                result += arg->toDouble();
            }
            return new Double(result);
        }
    }

    MinusFunction *const MinusFunction::object = new MinusFunction;
    const std::string MinusFunction::NAME("-");
    std::string MinusFunction::getName() {
        return NAME;
    }
    
    Object *MinusFunction::call(Arguments &args) {
        if (args.positionArgs() == 0) {
            argumentNumberError(args, 1);
        } else if (args.positionArgs() == 1) {
            Number *x = castArgument<Number>(args, 0);
            if (x->isInteger())
                return new Integer(-static_cast<Integer *>(x)->getVal());
            else
                return new Double(x->toDouble());
        } else {
            bool allInteger = true;
            for (int i = 0; i < args.positionArgs(); ++i) {
                if (typeid(*args.getArg(i)) != typeid(Integer)) {
                    allInteger = false;
                    break;
                }
            }
            if (allInteger) {
                // We can perform no checks because we already know that all of our arguments are Integer
                mpz_class result(static_cast<Integer *>(args.getArg(0))->getVal());
                for (int i = 1; i < args.positionArgs(); ++i)
                    result -= static_cast<Integer *>(args.getArg(i))->getVal();
                return new Integer(result);
            } else {
                double result = castArgument<Number>(args, 0)->toDouble();
                for (int i = 1; i < args.positionArgs(); ++i)
                    result -= castArgument<Number>(args, i)->toDouble();
                return new Double(result);
            }
        }
    }

    MultFunction *const MultFunction::object = new MultFunction;
    const std::string MultFunction::NAME("*");
    std::string MultFunction::getName() {
        return NAME;
    }
    Object *MultFunction::call(Arguments &args) {
        bool resultInteger = true;
        for (int i = 0; i < args.positionArgs(); ++i) {
            if (typeid(*args.getArg(i)) != typeid(Integer)) {
                resultInteger = false;
                break;
            }
        }
        if (resultInteger) {
            // We are already sure that all arguments types are Integer so no checks will be performed
            mpz_class result(1);
            for (int i = 0; i < args.positionArgs(); ++i)
                result *= static_cast<Integer *>(args.getArg(i))->getVal();
            return new Integer(result);
        } else {
            // we shall check that all of the arguments are either Integer or Double
            double result = 1.0;
            for (int i = 0; i < args.positionArgs(); ++i) {
                Number *arg = castArgument<Number>(args, i);
                result *= arg->toDouble();
            }
            return new Double(result);
        }
    }
    DivideFunction *const DivideFunction::object = new DivideFunction;
    const std::string DivideFunction::NAME("/");

    std::string DivideFunction::getName() {
        return NAME;
    }
    Object *DivideFunction::call(Arguments &args) {
        if (args.positionArgs() == 0) {
            argumentNumberError(args, 1);
        } else if (args.positionArgs() == 1) {
            return new Double(1.0 / castArgument<Number>(args, 0)->toDouble());
        } else {
            double result = castArgument<Number>(args, 0)->toDouble();
            for (int i = 1; i < args.positionArgs(); ++i)
                result /= castArgument<Number>(args, i)->toDouble();
            return new Double(result);
        }
    }

    IntegerDivideFunction *const IntegerDivideFunction::object = new IntegerDivideFunction;
    const std::string IntegerDivideFunction::NAME("div");

    std::string IntegerDivideFunction::getName() {
        return NAME;
    }

    Object *IntegerDivideFunction::call(Arguments &args) {
        if (args.positionArgs() <= 1) {
            argumentNumberError(args, 2);
        } else {
            mpz_class result(castArgument<Integer>(args, 0)->getVal());
            for (int i = 1; i < args.positionArgs(); ++i)
                result /= castArgument<Integer>(args, 1)->getVal();
            return new Integer(result);
        }
    }
    
    void init(Scope *scope) {
        add(PlusFunction::object, scope);
        add(MinusFunction::object, scope);
        add(MultFunction::object, scope);
        add(DivideFunction::object, scope);
        add(IntegerDivideFunction::object, scope);
    }
}


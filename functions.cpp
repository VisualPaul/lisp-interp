#include "functions.h"
#include "scope.h"

namespace {
    void add(Function *func, Scope *scope) {
        scope->addVariable(Symbol::getSymbol(func->getName()), func);
    }
}

namespace functions {
    const std::string PlusFunction::NAME("+");    
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

    const std::string MinusFunction::NAME("-");    
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

    const std::string MultFunction::NAME("*");
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
    
    const std::string DivideFunction::NAME("/");
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

    const std::string IntegerDivideFunction::NAME("div");
    Object *IntegerDivideFunction::call(Arguments &args) {
        if (args.positionArgs() <= 1) {
            argumentNumberError(args, 2);
        } else {
            mpz_class result(castArgument<Integer>(args, 0)->getVal());
            for (int i = 1; i < args.positionArgs(); ++i)
                mpz_fdiv_q(result.get_mpz_t(), result.get_mpz_t(), castArgument<Integer>(args, 1)->getVal().get_mpz_t());
            return new Integer(result);
        }
    }
    
    const std::string IntegerModFunction::NAME("mod");
    Object *IntegerModFunction::call(Arguments &args) {
        if (args.positionArgs() != 2)
            argumentNumberError(args, 2);
        mpz_class a = castArgument<Integer>(args, 0)->getVal();
        mpz_class b = castArgument<Integer>(args, 1)->getVal();
        mpz_class r;
        mpz_fdiv_r(r.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t());
        return new Integer(r);
    }
    const std::string GCDFunction::NAME("gcd");
    Object *GCDFunction::call(Arguments &args) {
        mpz_class result(0);
        for (int i = 0; i < args.positionArgs(); ++i)
            mpz_gcd(result.get_mpz_t(), result.get_mpz_t(), castArgument<Integer>(args, i)->getVal().get_mpz_t());
        return new Integer(result);
    }
    const std::string LCMFunction::NAME("lcm");
    Object *LCMFunction::call(Arguments &args) {
        mpz_class result(1);
        for (int i = 0; i < args.positionArgs(); ++i)
            mpz_lcm(result.get_mpz_t(), result.get_mpz_t(), castArgument<Integer>(args, i)->getVal().get_mpz_t());
        return new Integer(result);
    }
    const std::string EvalFunction::NAME("eval");
    Object *EvalFunction::call(Arguments &args) {
        if (args.positionArgs() != 1)
            argumentNumberError(args, 1);
        return args.getArg(0)->evalute(Scope::global);
    }

    const std::string ConsFunction::NAME("cons");
    Object *ConsFunction::call(Arguments &args) {
        if (args.positionArgs() != 2)
            argumentNumberError(args, 2);
        return new ConsCell(args.getArg(0), args.getArg(1));
    }
    
    void init(Scope *scope) {
        add(PlusFunction::object, scope);
        add(MinusFunction::object, scope);
        add(MultFunction::object, scope);
        add(DivideFunction::object, scope);
        add(IntegerDivideFunction::object, scope);
        add(IntegerModFunction::object, scope);
        add(GCDFunction::object, scope);
        add(LCMFunction::object, scope);
        add(EvalFunction::object, scope);
        add(ConsFunction::object, scope);
    }
}


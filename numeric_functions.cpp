#include "numeric_functions.h"

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
    const std::string NumberEqual::NAME("=");
    Object *NumberEqual::call(Arguments &args) { // TODO: Think of something to do with doubles
        if (args.positionArgs() == 0)
            argumentNumberError(args, 1);
        bool allInteger = true;
        for (int i = 0; i < args.positionArgs(); ++i) { 
            if (typeid(*args.getArg(i)) == typeid(Integer)) {
                allInteger = false;
                break;
            }
        }
        if (allInteger) {
            bool equal = true;
            for (int i = 0; i + 1 < args.positionArgs(); ++i) {
                equal = equal && (static_cast<Integer *>(args.getArg(i))->getVal() ==
                                  static_cast<Integer *>(args.getArg(i + 1))->getVal());
            }
            return lispBool(equal);
        } else {
            bool equal = true;
            for (int i = 0; i + 1 < args.positionArgs(); ++i) {
                equal = equal && (castArgument<Number>(args, i)->toDouble() ==
                                  castArgument<Number>(args, i + 1)->toDouble());
            }
            return lispBool(equal);
        }
    }
    const std::string NumberLess::NAME("<");
    Object *NumberLess::call(Arguments &args) { 
        for (int i = 0; i < args.positionArgs() -1 ; ++i) {
            Number *a = castArgument<Number>(args, i);
            Number *b = castArgument<Number>(args, i + 1);
            if (a->isInteger() && b->isInteger()) {
                Integer *ia = static_cast<Integer *>(a);
                Integer *ib = static_cast<Integer *>(b);
                if (ia->getVal() >= ib->getVal())
                    return lispBool(false);
            } else {
                if (a->toDouble() >= b->toDouble())
                    return lispBool(false);
            }
        }
        return lispBool(true);
    }

    const std::string NumberGreater::NAME(">");
    Object *NumberGreater::call(Arguments &args) {
        for (int i = 0; i < args.positionArgs() -1 ; ++i) {
            Number *a = castArgument<Number>(args, i);
            Number *b = castArgument<Number>(args, i + 1);
            if (a->isInteger() && b->isInteger()) {
                Integer *ia = static_cast<Integer *>(a);
                Integer *ib = static_cast<Integer *>(b);
                if (ia->getVal() <= ib->getVal())
                    return lispBool(false);
            } else {
                if (a->toDouble() <= b->toDouble())
                    return lispBool(false);
            }
        }
        return lispBool(true); 
    }

    const std::string NumberLessEqual::NAME("<=");
    Object *NumberLessEqual::call(Arguments &args) {
        for (int i = 0; i < args.positionArgs() -1 ; ++i) {
            Number *a = castArgument<Number>(args, i);
            Number *b = castArgument<Number>(args, i + 1);
            if (a->isInteger() && b->isInteger()) {
                Integer *ia = static_cast<Integer *>(a);
                Integer *ib = static_cast<Integer *>(b);
                if (ia->getVal() > ib->getVal())
                    return lispBool(false);
            } else {
                if (a->toDouble() > b->toDouble())
                    return lispBool(false);
            }
        }
        return lispBool(true); 
    }

    const std::string NumberGreaterEqual::NAME(">=");
    Object *NumberGreaterEqual::call(Arguments &args) {
        for (int i = 0; i < args.positionArgs() -1 ; ++i) {
            Number *a = castArgument<Number>(args, i);
            Number *b = castArgument<Number>(args, i + 1);
            if (a->isInteger() && b->isInteger()) {
                Integer *ia = static_cast<Integer *>(a);
                Integer *ib = static_cast<Integer *>(b);
                if (ia->getVal() < ib->getVal())
                    return lispBool(false);
            } else {
                if (a->toDouble() < b->toDouble())
                    return lispBool(false);
            }
        }
        return lispBool(true); 
    }
    
    void numeric_init(Scope *scope) {
        _add(PlusFunction::object, scope);
        _add(MinusFunction::object, scope);
        _add(MultFunction::object, scope);
        _add(DivideFunction::object, scope);
        _add(IntegerDivideFunction::object, scope);
        _add(IntegerModFunction::object, scope);
        _add(GCDFunction::object, scope);
        _add(LCMFunction::object, scope);
        _add(NumberEqual::object, scope);
        _add(NumberLess::object, scope);
        _add(NumberGreater::object, scope);
        _add(NumberGreaterEqual::object, scope);
        _add(NumberLessEqual::object, scope);
    }
}

// -*-c++-*-
#include "function.h"
#include "scope.h"

namespace functions {
    inline void _add(Function *func, Scope *scope) {
        scope->addVariable(Symbol::getSymbol(func->getName()), func);
    }
    template <class Func> class FunctionHelper : public Function {
    protected:
        FunctionHelper() {}
        ~FunctionHelper() {}
    public:
        std::string getName() override;
        static Func *const object;
    };
    template <class Func> std::string FunctionHelper<Func>::getName() {
        static_assert(std::is_base_of<FunctionHelper<Func>, Func>::value,
                      "FunctionHelper's template argument must be derived from the same FunctionHelper");
        return Func::NAME;
    }
    template <class Func> Func *const FunctionHelper<Func>::object = new Func;

    class EvalFunction : public FunctionHelper<EvalFunction> {
    public:
        static const std::string NAME;
        Object *call(Arguments &args) override;
    };
    class ConsFunction : public FunctionHelper<ConsFunction> {
    public:
        static const std::string NAME;
        Object *call(Arguments &args) override;
    };
    void numeric_init(Scope *scope);
    void init(Scope *scope);
};

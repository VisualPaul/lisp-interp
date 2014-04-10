// -*-c++-*-
#include "function.h"
#include "scope.h"

namespace functions {
    void _add(Function *func, Scope *scope) {
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

    class PlusFunction : public FunctionHelper<PlusFunction> {
    public:
        static const std::string NAME;
        Object *call(Arguments &args) override;
    };

    class MinusFunction : public FunctionHelper<MinusFunction> {
    public:
        static const std::string NAME;
        Object *call(Arguments &args) override;
    };
    class MultFunction : public FunctionHelper<MultFunction> {
    public:
        static const std::string NAME;
        Object *call(Arguments &args) override;
    };
    class DivideFunction : public FunctionHelper<DivideFunction> {
    public:
        static const std::string NAME;
        Object *call(Arguments &args) override;
    };
    class IntegerDivideFunction : public FunctionHelper<IntegerDivideFunction> {
    public:
        static const std::string NAME;
        Object *call(Arguments &args) override;
    };
    class IntegerModFunction : public FunctionHelper<IntegerModFunction> {
    public:
        static const std::string NAME;
        Object *call(Arguments &args) override;
    };
    class GCDFunction : public FunctionHelper<GCDFunction> {
    public:
        static const std::string NAME;
        Object *call(Arguments &args) override;
    };
    class LCMFunction : public FunctionHelper<LCMFunction> {
    public:
        static const std::string NAME;
        Object *call(Arguments &args) override;
    };
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
    void init(Scope *scope);
};

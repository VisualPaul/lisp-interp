// -*-c++-*-
#include "function.h"

namespace functions {
    template <class Func> class FunctionHelper : public Function {
        //        static_assert(std::is_base_of<FunctionHelper<Func>, Func>::value,
        //                      "FunctionHelper's template argument must be derived from the same FunctionHelper");
    protected:
        FunctionHelper() {}
        ~FunctionHelper() {}
    public:
        std::string getName() override;
        static Func *const object;
    };
    template <class Func> std::string FunctionHelper<Func>::getName() {
        return Func::NAME;
    }
    template <class Func> Func *const FunctionHelper<Func>::object = new Func;

    class PlusFunction : public Function {
        PlusFunction() {}
        ~PlusFunction() {}
    public:
        static PlusFunction *const object;
        static const std::string NAME;
        Object *call(Arguments &args) override;
        std::string getName() override;
    };

    class MinusFunction : public Function {
        MinusFunction() {}
        ~MinusFunction() {}
    public:
        static MinusFunction *const object;
        static const std::string NAME;

        Object *call(Arguments &args) override;
        std::string getName() override;
    };
    class MultFunction : public Function {
        MultFunction() {}
        ~MultFunction() {}
    public:
        static MultFunction *const object;
        static const std::string NAME;

        Object *call(Arguments &args) override;
        std::string getName() override;
    };
    class DivideFunction : public Function {
        DivideFunction() {}
        ~DivideFunction() {}
    public:
        static DivideFunction *const object;
        static const std::string NAME;
        Object *call(Arguments &args) override;
        std::string getName() override;
    };
    class IntegerDivideFunction : public Function {
        IntegerDivideFunction()  {}
        ~IntegerDivideFunction() {}
    public:
        static IntegerDivideFunction *const object;
        static const std::string NAME;
        Object *call(Arguments &args) override;
        std::string getName() override;
    };
    class IntegerModFunction : public Function {
        IntegerModFunction() {}
        ~IntegerModFunction() {}
    public:
        static IntegerModFunction *const object;
        static const std::string NAME;
        Object *call(Arguments &args) override;
        std::string getName() override;
    };
    class GCDFunction : public Function {
        GCDFunction() {}
        ~GCDFunction() {}
    public:
        static GCDFunction *const object;
        static const std::string NAME;
        Object *call(Arguments &args) override;
        std::string getName() override;
    };
    class LCMFunction : public Function {
        LCMFunction() {}
        ~LCMFunction() {}
    public:
        static LCMFunction *const object;
        static const std::string NAME;
        Object *call(Arguments &args) override;
        std::string getName() override;
    };
    class EvalFunction : public Function {
        EvalFunction() {}
        ~EvalFunction() {}
    public:
        static EvalFunction *const object;
        static const std::string NAME;
        Object *call(Arguments &args) override;
        std::string getName() override;
    };
    class ConsFunction : public FunctionHelper<ConsFunction> {
    public:
        static const std::string NAME;
        Object *call(Arguments &args) override;
    };
    void init(Scope *scope);
};

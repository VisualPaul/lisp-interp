// -*-c++-*-
#include "function.h"

namespace functions {
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

    void init(Scope *scope);
};

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

    class MinusFunction : public  Function {
        MinusFunction() {}
        ~MinusFunction() {}
    public:
        static MinusFunction *const object;
        static const std::string NAME;

        Object *call(Arguments &args) override;
        std::string getName() override;
    };

    void init(Scope *scope);
};

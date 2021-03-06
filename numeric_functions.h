// -*-c++-*-
#ifndef NUMERIC_FUNCTIONS_H
#define NUMERIC_FUNCTIONS_H
#include "functions.h"

namespace functions {
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
    class NumberEqual : public FunctionHelper<NumberEqual> {
    public:
        static const std::string NAME;
        Object *call(Arguments &args) override;
    };
    class NumberLess : public FunctionHelper<NumberLess> {
    public:
        static const std::string NAME;
        Object *call(Arguments &args) override;
    };
    class NumberGreater : public FunctionHelper<NumberGreater> {
    public:
        static const std::string NAME;
        Object *call(Arguments &args) override;
    };
    class NumberLessEqual : public FunctionHelper<NumberLessEqual> {
    public:
        static const std::string NAME;
        Object *call(Arguments &args) override;
    };
    class NumberGreaterEqual : public FunctionHelper<NumberGreaterEqual> {
    public:
        static const std::string NAME;
        Object *call(Arguments &args) override;
    };
}
#endif /* NUMERIC_FUNCTIONS_H */

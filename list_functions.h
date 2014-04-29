// -*-c++-*-
#ifndef LIST_FUNCTIONS_H
#define LIST_FUNCTIONS_H

#include "functions.h"

namespace functions {
    class ConsFunction : public FunctionHelper<ConsFunction> {
    public:
        static const std::string NAME;
        Object *call(Arguments &args) override;
    };
    class CarFunction : public FunctionHelper<CarFunction> {
    public:
        static const std::string NAME;
        Object *call(Arguments &args) override;
    };
    class CdrFunction : public FunctionHelper<CdrFunction> {
    public:
        static const std::string NAME;
        Object *call(Arguments &args) override;
    };
    declareFunctionHelper(ListLengthFunction);
}

#endif /* LIST_FUNCTIONS_H */

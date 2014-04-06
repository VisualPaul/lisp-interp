// -*-c++-*-
#ifndef FUNCTION_H
#define FUNCTION_H
#include "object.h"
#include <type_traits>

class Arguments {
public:    
    Arguments(Object *argList) : _args(ConsCell::toVector(argList)) {}
    std::vector<Object *> getPositionArgs() {return _args; }
    Object *getArg(int idx) {
        if (idx < 0 || (size_t)idx > _args.size())
            error("incorrect argument index: %d", idx);
        return _args[idx];
    }
    int positionArgs() {
        return static_cast<int>(_args.size());
    }
private:
    std::vector<Object *> _args;
};

class Function : public Object {
public:
    virtual Object *evalute(Scope *scope);
    Function *clone() override;
    virtual Object *call(Arguments &args) = 0;
    virtual std::string getName() = 0;
    std::string getTypeName() override;
    void print(std::ostream &out) override;
    static const std::string NAME;
protected:
    void reportTypeError(Arguments &args, int argNumber, std::string expected) __attribute__((noreturn)) {
        error("type mismatch at %s call: expected %s got %s as %d argument", getName().c_str(),
              expected.c_str(), args.getArg(argNumber)->getTypeName().c_str(), argNumber);
    }

    template <class ResultType>
    ResultType *castArgument(Arguments &args, int argNumber) {
        static_assert(std::is_base_of<Object, ResultType>::value, "castArgument result type must be derived from Object");
        ResultType *result = dynamic_cast<ResultType *>(args.getArg(argNumber));
        if (!result)
            reportTypeError(args, argNumber, ResultType::TYPE_NAME);
        return result;
    }

    void argumentNumberError(Arguments &args, int expected) __attribute__((noreturn)) {
        error("wrong number of arguments at %s call: expected %d got %d", getName().c_str(), expected, args.positionArgs());
    }

    ~Function();
};


#endif

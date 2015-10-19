#include "function.h"
#include "scope.h"
#include <memory>

Function *Function::clone() {
    return this;
}

const std::string Function::TYPE_NAME("Function");
std::string Function::getTypeName() {
    return TYPE_NAME;
}

void Function::print(std::ostream &out) {
    out << "#FUNCTION<" << getName() << ">";
}

Object *Function::evalute(Scope *) {
    error("Cannot evalute function");
}

bool Function::isMacro() {
    return false;
}

Function::~Function() {}

UserDefinedFunction::UserDefinedFunction(const std::string &name, Scope *baseScope,
                                         const std::vector<Symbol *> &args, Object *body, bool macro, Symbol *rest)
    : _name(name), _baseScope(baseScope), _argumentsName(args), _body(body), _restArgumentName(rest), _macro(macro) {
    if (!body->isList())
        error("function body argument must be list");
}

UserDefinedFunction::UserDefinedFunction(const std::string &name, Scope *baseScope,
                                         std::vector<Symbol *> &&args, Object *body, bool macro, Symbol *rest)
    : _name(name), _baseScope(baseScope), _argumentsName(args), _body(body), _restArgumentName(rest), _macro(macro) {
    if (!body->isList())
        error("function body argument must be list");
}

std::string UserDefinedFunction::getName() {
    return _name;
}

Object *UserDefinedFunction::call(Arguments &args) {
    std::unique_ptr<Scope> functionScope(new Scope(_baseScope));
    int argumentNumber = static_cast<int>(_argumentsName.size());
    if (args.positionArgs() < argumentNumber || (args.positionArgs() > argumentNumber && _restArgumentName == nullptr))
        argumentNumberError(args, static_cast<int>(_argumentsName.size()));
    for (int i = 0; i < static_cast<int>(_argumentsName.size()); ++i)
        functionScope->addVariable(_argumentsName[i], args.getArg(i));
    Object *restArgs = NullObject::getNullObject();
    for (int i = args.positionArgs() - 1; i >= static_cast<int>(_argumentsName.size()); --i)
        restArgs = new ConsCell(args.getArg(i), restArgs);
    if (_restArgumentName != nullptr)
        functionScope->addVariable(_restArgumentName, restArgs);
    Object *result = NullObject::getNullObject();
    ListIterator it(_body);
    while (it.hasNext())
        result = it.next()->evalute(functionScope.get());
    return result;
}

bool UserDefinedFunction::isMacro() {
    return _macro;
}

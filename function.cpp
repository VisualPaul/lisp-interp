#include "function.h"

Function *Function::clone() {
    return this;
}

const std::string Function::NAME("Function");
std::string Function::getTypeName() {
    return NAME;
}

void Function::print(std::ostream &out) {
    out << "#FUNCTION<" << getName() << ">";
}

Object *Function::evalute(Scope *) {
    error("Cannot evalute function");
}

Function::~Function() {}

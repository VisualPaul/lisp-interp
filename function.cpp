#include "function.h

Function *Function::clone() {
    return this;
}

void Function::print(std::ostream &out) {
    out << "#FUNCTION<" << getName() << ">";
}

Object *Function::evalute(Scope *) {
    error("Cannot evalute function");
}

Function::~Function() {}

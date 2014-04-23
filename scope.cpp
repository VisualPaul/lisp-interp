#include "scope.h"

namespace functions {
    void init(Scope *); 
}

Scope::Scope() : _parent(nullptr) {
    functions::init(this);
}

Scope::Scope(Scope *parentScope) : _parent(parentScope) {
    if (!parentScope)
        error("parent scope must not be null at Scope::Scope(Scope *)");
}

Scope *Scope::_global = nullptr;

Scope *Scope::global() {
    if (!_global)
	_global = new Scope;
    return _global; 
}

void Scope::setVariable(Symbol *sym, Object *val) {
    if (val == nullptr)
        error("Scope::setVaraible value argument is null pointer");
    for (Scope *cur = this; cur; cur = cur->parentScope()) {
        auto it = cur->_m.find(sym->getId());
        if (it != cur->_m.end()) {
            it->second = val;
            return;
        } 
    }
    addVariable(sym, val);
}

Object *Scope::getVariable(Symbol *sym) {
    for (Scope *cur = this; cur != nullptr; cur = cur->parentScope()) {
        auto it = cur->_m.find(sym->getId());
        if (it != cur->_m.end())
            return it->second;
    }
    return nullptr;
}

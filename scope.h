// -*-c++-*-
#ifndef SCOPE_H
#define SCOPE_H
#include "object.h"

class Scope {
public:
    Scope(Scope *parent);
    Object *getVariable(Symbol *sym);
    void setVariable(Symbol *sym, Object *value);
    void addVariable(Symbol *sym, Object *value) {
        _m.emplace(sym->getId(), value);
    }
    Scope *parentScope() {
        return _parent;
    }
    Scope *dropScope() {
        Scope *result = parentScope();
        delete this;
        return result;
    }
    bool hasVariable(Symbol *sym) {
        return getVariable(sym) != nullptr;
    }
    
    ~Scope() {}
private:
    std::unordered_map<int, Object *> _m;
    Scope *_parent;

};

#endif /* SCOPE_H */

// -*-c++-*-
#ifndef SCOPE_H
#define SCOPE_H
#include "object.h"

class Scope : public GCObject {
    Scope();
    Scope(Scope *parent);
public:
    static Scope *newScope(Scope *parent) {
        return new Scope(parent);
    }
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
        return result;
    }
    bool hasVariable(Symbol *sym) {
        return getVariable(sym) != nullptr;
    }
    static Scope *global();

    std::string gcRepr() override;
protected:
    void gcMarkChildren() override;
private:
    ~Scope() override;
    std::unordered_map<int, Object *> _m;
    Scope *_parent;
    static GCObjectPtr<Scope> _global;
};

#endif /* SCOPE_H */

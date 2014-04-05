#include "special_forms.h"
#include <memory>

namespace special_forms {
    Object *quoteSpecial(Object *args, Scope *scope) {
        if (!args->isList()) {
            error("incorrect argument to function quoteSpecial: expected list");
        }
        if (listLength(args) != 1) {
            error("wrong number of arguments to quote special form");
        }
        return dynamic_cast<ConsCell &>(*args).car();
    }

    Object *ifSpecial(Object *args, Scope *scope) {
        if (!args->isList()) {
            error("incorrect argument to function ifSpecial: expected list");
        }
        int len = listLength(args);
        if (len != 2 && len != 3)
            error("wrong number of arguments to if special form");
        auto vargs = ConsCell::toVector(args);
        Object *test = v[0];
        Object *then = v[1];
        Object *otherwise = NullObject::null;
        if (v.size() == 3)
            otherwise = v[2];        
        if (!test->evalute(scope)->isNull())
            return then->evalute(scope);
        else
            return otherwise->evalute(scope());
    }

    Object *letSpecial(Object *args, Scope *scope) {
        std::unique_ptr<Scope> newScope(new Scope(scope));
        if (!args->isList()) {
            error("incorrect argument to function letSpecial: expected list");
        }
        if (!listAtLeast(args, 1)) {
            error("incorrect number of arguments to let special form");
        }
        Object *bindings = listGet(args, 0);
        if (!bindings->isList()) {
            error("mailformed bindings for let special form");
        }
        for (ListIterator it(bindings); it.hasNext();) {
            Object *bind = it.next();
            if (bind->isList()) {
                int len = listLength(bind);
                if (len != 1 && len != 2)
                    error("mailformed bind for let special form");
                Symbol *name = dynamic_cast<Symbol *>(listGet(bind, 0));
                if (!name)
                    error("mailformed bind for let special form");
                Object *value = NullObject::null;
                if (len == 2)
                    value = listGet(bind, 1)->evalute(newScope.get());
                newScope->addVariable(name, value);
            } else {
                Symbol *name = dynamic_cast<Symbol *>(bind);
                if (!name)
                    error("mailformed bind for let special form");
                newScope->addVariable(name, NullObject::null);
            }
        }
        return progSpecial(static_cast<ConsCell *>(args)->cdr(), newScope.get());
    }
    Object *progSpecial(Object *args, Scope *scope) {
        if (!args->isList()) {
            error("incorrect argument to function progsSpecial: expected list");
        }
        Object *result = NullObject::null;
        for (ListIterator it(args); it.hasNext(); ) {
            result = it.next()->evalute(scope);
        }
        return result;
    }
    Symbol *const quoteSymbol = Symbol::getSymbol("quote");
    Symbol *const ifSymbol    = Symbol::getSymbol("if");
    Symbol *const letSymbol   = Symbol::getSymbol("let");
    Symbol *const progSymbol  = Symbol::getSymbol("prog");
}

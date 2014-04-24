#include "special_forms.h"
#include "scope.h"
#include "function.h"
#include <memory>

namespace special_forms {
    Object *quoteSpecial(Object *args, Scope *) {
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
        Object *test = vargs[0];
        Object *then = vargs[1];
        Object *otherwise = NullObject::null;
        if (vargs.size() == 3)
            otherwise = vargs[2];        
        if (!test->evalute(scope)->isNull())
            return then->evalute(scope);
        else
            return otherwise->evalute(scope);
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
    Object *lambdaSpecial(Object *args, Scope *scope) {
        if (!args->isList()) {
            error("incorrect argument to function lambdaSpecial: expected list");
        }
        if (!listAtLeast(args, 1)) {
            error("lambda special form must contain an argument list");
        }
        ListIterator it(args);
        Object *bindings = it.next();
	String *functionNameObj = dynamic_cast<Function *>(bindings);
	std::string functionName("lambda");
	if (functionNameObj != nulllptr) {
	    bindings = it.next();
	    functionName = functionNameObj->getString();
	}
	
        std::vector<Symbol *> argumentNames;
        ListIterator argIt(bindings);
        while (argIt.hasNext()) {
            Symbol *sym = dynamic_cast<Symbol *>(argIt.next());
            if (sym == nullptr)
                error("mailformed bindings for lambda special form: symbol expected");
            argumentNames.push_back(sym);
        }
        return new UserDefinedFunction(functionName, scope, argumentNames, it.getObject());
    }
    Object *setSpecial(Object *args, Scope *scope) {
        if (!args->isList()) {
            error("incorrect argument to function setSpecial: expected list");
        }
        int n = listLength(args);
        if (n % 2 != 0)
	  error("set special forms must contain even number of arguments");
        ListIterator it(args);
        Object *res = NullObject::null;
        while (it.hasNext()) {
            Symbol *name = dynamic_cast<Symbol *>(it.next()->evalute(scope));
            if (name == nullptr)
                error("set special form arguments must be symbols");
            Object *val = it.next()->evalute(scope);
            res = val;
            scope->setVariable(name, val);
        }
        return res;
    }
    Object *defvarSpecial(Object *args, Scope *scope) {
        if (!args->isList())
            error("incorrect argument to function defvarSpecial: expected list");
        if (listLength(args) % 2 != 0)
            error("defvar special form must contain even number of arguments");
        ListIterator it(args);
        Object *res = NullObject::null;
        while (it.hasNext()) {
            Symbol *name = dynamic_cast<Symbol *>(it.next()->evalute(scope));
            if (name == nullptr)
                error("defvar special form argument must be a symbol");
            Object *val = it.next()->evalute(scope);
            res = val;
            scope->addVariable(name, val);
        }
        return res;
    }
    Symbol *const quoteSymbol  = Symbol::getSymbol("quote");
    Symbol *const ifSymbol     = Symbol::getSymbol("if");
    Symbol *const letSymbol    = Symbol::getSymbol("let");
    Symbol *const progSymbol   = Symbol::getSymbol("prog");
    Symbol *const lambdaSymbol = Symbol::getSymbol("lambda");
    Symbol *const setSymbol    = Symbol::getSymbol("set");
    Symbol *const defvarSymbol = Symbol::getSymbol("defvar");
}

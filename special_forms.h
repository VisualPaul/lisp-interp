// -*-c++-*-
#ifndef SPECIAL_FORMS_H
#define SPECIAL_FORMS_H
#include "object.h"

namespace special_forms {
    Object *quoteSpecial(Object *args, Scope *scope);
    Object *ifSpecial(Object *args, Scope *scope);
    Object *letSpecial(Object *args, Scope *scope);
    Object *progSpecial(Object *args, Scope *scope);
    Object *lambdaSpecial(Object *args, Scope *scope);
    Object *macroSpecial(Object *args, Scope *scope);
    Object *setSpecial(Object *args, Scope *scope);
    Object *defvarSpecial(Object *args, Scope *scope);
    Object *whileSpecial(Object *args, Scope *scope);
    extern Symbol *const quoteSymbol;
    extern Symbol *const ifSymbol;
    extern Symbol *const letSymbol;
    extern Symbol *const progSymbol;
    extern Symbol *const lambdaSymbol;
    extern Symbol *const macroSymbol;
    extern Symbol *const setSymbol;
    extern Symbol *const defvarSymbol;
    extern Symbol *const whileSymbol;
}
#endif

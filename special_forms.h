// -*-c++-*-
#ifndef SPECIAL_FORMS_H
#define SPECIAL_FORMS_H
#include "object.h"

namespace special_forms {
    Object *quoteSpecial(Object *args, Scope *scope);
    Object *ifSpecial(Object *args, Scope *scope);
    Object *letSpecial(Object *args, Scope *scope);
    Object *progSpecial(Object *args, Scope *scope);
    Object *lambdaSpecial(Object *ars, Scope *scope);
    extern Symbol *const quoteSymbol;
    extern Symbol *const ifSymbol;
    extern Symbol *const letSymbol;
    extern Symbol *const progSymbol;
    extern Symbol *const lambdaSymbol;
}
#endif

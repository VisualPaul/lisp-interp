#include "object.h"
#include "special_forms.h"
#include "function.h"
#include "scope.h"
#include <cctype>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <boost/regex.hpp>
#include <iostream>

bool Object::isList() {
    return false;
}

bool Object::isNull() {
    return false;
}

Object *Object::evalute(Scope *) {
    return this;
}

void Object::free() {
    delete this;
}

Object::~Object() {
}

Symbol *Symbol::clone() {
    return this; 
}

void Symbol::free() {
}

Symbol *Symbol::getSymbol(const std::string &str) {
    auto it = _m.find(str);
    if (it != _m.end())
        return it->second;
    Symbol *symbol = new Symbol;
    symbol->_sym = _m.size();
    _m.emplace(str, symbol);
    _v.push_back(str);
    return symbol;
}

const std::string Symbol::TYPE_NAME("Symbol");

std::string Symbol::getTypeName() {
    return TYPE_NAME;
}

std::string Symbol::getText() {
    return _v[_sym];
}

Object *Symbol::evalute(Scope *scope) {
    Object *result = scope->getVariable(this);
    if (result == nullptr)
        error("no such variable: %s", getText().c_str());
    return result;
}

void Symbol::print(std::ostream &out) {
    out << getText();
}

Symbol::~Symbol() {
}
std::unordered_map<std::string, Symbol *> Symbol::_m;
std::vector<std::string> Symbol::_v;

String *String::clone() {
    return new String(_str);
}

void String::print(std::ostream &out) {
    out << '"' << escape(getString()) << '"';
}

const std::string String::TYPE_NAME("String");

std::string String::getTypeName() {
    return TYPE_NAME;
}

String::~String() {
}

Character *Character::clone() {
    return new Character(_c);
}

void Character::print(std::ostream &out) {
    out << "'" << escape(getChar()) << "'";
}

const std::string Character::TYPE_NAME("Character");

std::string Character::getTypeName() {
    return TYPE_NAME;
}

Character::~Character() {

}

const std::string Number::TYPE_NAME("Number");

Double::Double(const std::string &str) : _val(strtod(str.c_str(), nullptr)) {
}

Double * Double::clone() {
    return new Double(_val);
}

bool Double::checkString(const std::string &str) {
    bool hasDigit = false;
    for (size_t i = 0; i < str.size(); ++i) {
        if (isdigit(str[i])) {
            hasDigit = true;
            break;
        }
    }
    if (!hasDigit)
        return false;
    return boost::regex_match(str, boost::regex(R"([+-]?\d*(\.\d*)?([eE][+-]?\d+)?)"));
}

bool Double::isInteger() {
    return false;
}

double Double::toDouble() {
    return getVal();
}

void Double::print(std::ostream &str) {
    str << _val;
}

const std::string Double::TYPE_NAME("Double");

std::string Double::getTypeName() {
    return TYPE_NAME;
}

Double::~Double() {
}

bool Integer::checkString(const std::string &str) {
    return boost::regex_match(str, boost::regex(R"([+-]?(0[0-7]*|0x[0-9a-fA-F]+|0b[01]+|[0-9]+))"));
}

Integer *Integer::clone() {
    return new Integer(_val);
}

void Integer::print(std::ostream &out) {
    out << _val;
}

const std::string Integer::TYPE_NAME("Integer");

std::string Integer::getTypeName() {
    return TYPE_NAME;
}

bool Integer::isInteger() {
    return true;
}

double Integer::toDouble() {
    return _val.get_d();
}

Integer::~Integer() {
}

NullObject *NullObject::clone() {
    return this;
}

void NullObject::free() {
}

NullObject *const NullObject::null = new NullObject;

bool NullObject::isList() {
    return true;
}

bool NullObject::isNull() {
    return true;
}

void NullObject::print(std::ostream &out) {
    out << "nil";
}

const std::string NullObject::TYPE_NAME("Null");

std::string NullObject::getTypeName() {
    return TYPE_NAME;
}

NullObject::~NullObject() {
}

ConsCell *ConsCell::clone() {
    return new ConsCell(_car, _cdr);
}

Object *ConsCell::fromVector(const std::vector<Object *> &vc)
{
    Object *result = NullObject::null;
    for (auto it = vc.rbegin(); it != vc.rend(); ++it)
        result = new ConsCell(*it, result);
    return result;
}

std::vector<Object *> ConsCell::toVector(Object *lst) {
    ListIterator it(lst);
    std::vector<Object *> result;
    while (it.hasNext())
        result.push_back(it.next());
    return result;
}

void ConsCell::print(std::ostream &out) {
    if (!isList()) {
        out << "(";
        car()->print(out);
        out << " . ";
        cdr()->print(out);
        out << ")";
    } else {
        out << "(";
        car()->print(out);
        Object *ptr = cdr();
        while (!ptr->isNull()) {
            ConsCell *cur = dynamic_cast<ConsCell *>(ptr);
            if (!cur)
                error("something went awfully wrong inside ConsCell::print");
            out << ' ';
            cur->car()->print(out);
            ptr = cur->cdr();
        }
        out << ')';
    }
        
}

const std::string ConsCell::TYPE_NAME("Cons");
std::string ConsCell::getTypeName() {
    return TYPE_NAME;
}

Object *ConsCell::evalute(Scope *scope) {
    if (!isList())
        error("Cannot evalute cons cell");
    Symbol *smb = dynamic_cast<Symbol *>(car());
    if (smb != nullptr) {
        using namespace special_forms;
        if (smb == quoteSymbol) {
            return quoteSpecial(cdr(), scope);
        } else if (smb == ifSymbol) {
            return ifSpecial(cdr(), scope);
        } else if (smb == letSymbol) {
            return letSpecial(cdr(), scope);
        } else if (smb == progSymbol) {
            return progSpecial(cdr(), scope);
        }
    }
    Function *func = dynamic_cast<Function *>(car()->evalute(scope));
    if (func == nullptr)
        error("first object in Lisp form must be either special form name or function");
    std::vector<Object *> argsVector;
    ListIterator it(cdr());
    while (it.hasNext())
        argsVector.push_back(it.next()->evalute(scope));
    Arguments args(argsVector);
    return func->call(args);
    
}

bool ConsCell::isList() {
    return cdr()->isList();
}

ConsCell::~ConsCell() {
}

int listLength(Object *list) {
    if (!list->isList())
        error("listLength argument is not list");
    int result = 0;
    for (ListIterator it(list); it.hasNext(); it.next()) {
        ++result;
    }
    return result;
}

bool listAtLeast(Object *list, int minSize) {
    if (minSize < 0)
        error("listAtLeast argument is negative");
    if (!list->isList())
        error("listAtLeast argument is not list");
    ListIterator it(list);
    for (int i = 0; i < minSize; ++i) {
        if (!it.hasNext())
            return false;
        it.next();
    }
    return true;
}

Object *listGet(Object *list, int idx) {
    if (idx < 0)
        error("listGet argument is negative");
    if (!list->isList())
        error("listGet argument is not list");
    ListIterator it(list);
    for (int i = 0; i < idx; ++i) {
        if (!it.hasNext())
            error("listGet argument is greater than list size");
        it.next();
    }
    if (!it.hasNext())
        error("listGet argument is greater than list size");
    return it.next();
}

std::istream &skipStreamSpaces(std::istream &in) {
    while (!in.eof()) {
        if (!isspace(in.get())) {
            in.unget();
            break;
        }
    }
    return in;
}

void error(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    putc('\n', stderr);
    va_end(ap);
    abort();
}

std::string escape(char c) {
    unsigned int code = static_cast<unsigned char>(c);
    if (code > 127 || (!isgraph(c) && !isspace(c))) {
        char str[5];
        sprintf(str, "\\x%x%x", (code >> 4), (code & 0x0F));
        return std::string(str);
    } else if (isgraph(c) || c == ' ') {
        return std::string(1, c);
    } else if (c == '\n') {
        return std::string("\\n");
    } else if (c == '\t') {
        return std::string("\\t");
    } else if (c == '\v') {
        return std::string("\\v");
    } else if (c == '\r') {
        return std::string("\\r");
    } else if (c == '\f') {
        return std::string("\\f");
    } else {
        error("unknown character code to escape: %d", (int) c);
    }
}

std::string escape(const std::string &s) {
    std::string result;
    for (char c : s) {
        result += escape(c);
    }
    return result;
}

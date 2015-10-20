// -*-c++-*-
#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <gmpxx.h>
#include <string>
#include "gc.h"

class Scope;

class Object : public GCObject {
public:
    std::string gcRepr() override;
    Object() {}
    virtual Object *clone() = 0;
    virtual void print(std::ostream &out) = 0;

    virtual bool isList();
    virtual bool isNull();
    virtual Object *evalute(Scope *scope);
    virtual std::string getTypeName() = 0;
};

class Symbol : public Object {
public:
    Symbol *clone() override;
    static Symbol *getSymbol(const std::string &str);
    std::string getText();
    int getId() { return static_cast<int>(_sym); }
    void print(std::ostream &out) override;
    Object *evalute(Scope *scope) override;
    std::string getTypeName() override;
    static const std::string TYPE_NAME;

    void operator =(const Symbol &) = delete;
    Symbol(const Symbol &) = delete;
protected:
    ~Symbol();
private:
  
    static std::unordered_map<std::string, Symbol *> *_map;
    static std::unordered_map<std::string, Symbol *> *_m() {
        if (!_map)
            _map = new std::unordered_map<std::string, Symbol *>();
        return _map;
    }
    static std::vector<std::string> &_v() {
        if (!_vec)
            _vec = new std::vector<std::string>;
        return *_vec;
    }
    static std::vector<std::string> *_vec;
    size_t _sym;
    Symbol() {}
    friend class GC;
    static void gcMarkSymbols();
};

class String : public Object {
public:
    String(const std::string &str) : _str(str) {}
    String(std::string &&str) : _str(str) {}
    String *clone() override;
    void print(std::ostream &out) override;

    std::string getTypeName() override;
    static const std::string TYPE_NAME;

    std::string getString() { return _str; }
private:
    ~String();
    std::string _str;
};

class Character : public Object {
public:
    Character(char c) : _c(c) {}
    Character *clone() override;
    void print(std::ostream &out) override;

    std::string getTypeName() override;
    static const std::string TYPE_NAME;

    char getChar() { return _c; }
private:
    ~Character();
    char _c;
};

class Number : public Object {
public:
    static const std::string TYPE_NAME;
    virtual bool isInteger() = 0;
    virtual double toDouble() = 0;
};

class Integer : public Number {
public:
    Integer(long int x) : _val(x) {}
    Integer(unsigned long x) : _val(x) {}
    Integer(double x) : _val(x) {}
    Integer(const std::string &str) : _val(str) {}
    Integer(const mpz_class &x) : _val(x) {}
    Integer(mpz_class &&x) : _val(x) {}

    static bool checkString(const std::string &str);

    Integer *clone() override;
    void print(std::ostream &out) override;

    std::string getTypeName() override;
    static const std::string TYPE_NAME;

    bool isInteger() override;
    double toDouble() override;

    mpz_class getVal() {
        return _val;
    }
private:
    ~Integer();
    mpz_class _val;
};

class Double : public Number {
public:
    Double(double val) : _val(val) {}
    Double(const std::string &str);
    Double *clone() override;
    static bool checkString(const std::string &str);
    void print(std::ostream &out) override;

    std::string getTypeName() override;
    static const std::string TYPE_NAME;

    bool isInteger() override;
    double toDouble() override;

    double getVal() { return _val; }

private:
    ~Double();
    double _val;
};

class NullObject : public Object {
public:
    NullObject(const NullObject &) = delete;
    void operator =(const NullObject &) = delete;
    NullObject *clone() override;
    void print(std::ostream &out) override;
    bool isList() override;
    bool isNull() override;

    std::string getTypeName() override;
    static const std::string TYPE_NAME;

    static NullObject *getNullObject() {
        if (_object.getNormalPointer() == nullptr)
            _object = new NullObject;
        return _object.getNormalPointer();
    }
protected:
    NullObject()  {}
    ~NullObject();
private:
    static GCObjectPtr<NullObject> _object;
};

class ConsCell : public Object {
public:
    static Object *fromVector(const std::vector<Object *> &vec);
    static std::vector<Object *> toVector(Object *list);
    ConsCell(Object *car, Object *cdr) : _car(car), _cdr(cdr) {}
    Object *evalute(Scope *scope) override;
    Object *car() {
        return _car;
    }
    Object *cdr() {
        return _cdr;
    }
    void setCar(Object *car) {
        _car = car;
    }
    void setCdr(Object *cdr) {
        _cdr = cdr;
    }
    bool isList() override;
    ConsCell *clone() override;

    void print(std::ostream &str) override;
    std::string getTypeName() override;
    static const std::string TYPE_NAME;
protected:
    void gcMarkChildren() override;
private:
    ~ConsCell();
    Object *_car, *_cdr;
};

inline Object *lispBool(bool x) {
    static Object *const trueValue = Symbol::getSymbol("t");
    return x? trueValue : NullObject::getNullObject();
}

int listLength(Object *list);
bool listAtLeast(Object *list, int minSize);
Object *listGet(Object *list, int x);

//utility functions for error handling
void error(const char *fmt, ...) __attribute__((noreturn));

class ListIterator {
public:
    ListIterator(Object *p = NullObject::getNullObject()) : _p(p) {
        if (!p->isList())
            error("incorrect argument to list iterator ctor: list expected");
    }
    bool hasNext() {
        return !_p->isNull();
    }
    Object *next() {
        if (!hasNext())
            error("incorrect next in list iterator class");
        ConsCell *cur = &dynamic_cast<ConsCell &>(*_p);
        _p = cur->cdr();
        return cur->car();
    }
    Object *getObject() {
        return _p;
    }
private:
    Object *_p;
};

std::string escape(char c);
std::string escape(const std::string &s);

// utility functions to work with istreams
std::istream &skipStreamSpaces(std::istream &str);

#endif

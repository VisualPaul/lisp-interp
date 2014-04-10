// -*-c++-*-
#ifndef PARSER_H
#define PARSER_H
#include "object.h"

class Parser {
public:
    Parser(std::istream &in) : _istr(in) {}
    Parser(const std::string &str);
    Object *nextExpression();
    int lineNumber() { return _lineno; }
private:
    int _lineno;
    std::istream &_istr;
    enum _TokenType {
        TT_NORMAL, TT_OPEN, TT_CLOSE, TT_NO, TT_QUOTE
    };
    class _Token {
        int _type;
        Object *_obj;
    public:
        _Token(int type) : _type(type) {
            if (type == TT_NORMAL)
                error("cannot create token of TT_NORMAL type without specifying object");
        }
        _Token(Object *obj) : _type(TT_NORMAL), _obj(obj) {}
        int getType() { return _type; }
        Object *getObject() {
            if (_type != TT_NORMAL)
                error("only can get object from TT_NORMAL type object");
            return _obj;
        }
    };
    _Token nextToken();
    void skipSpaces();
    char next();
    char peek();
    char getEscape();
};

#endif /* PARSER_H */

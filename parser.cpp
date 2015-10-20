#include "parser.h"
#include <cstring>
#include <cctype>

char Parser::next() {
    if (_istr.eof())
        error("incorrect Parser::next() call: EOF found");
    int c = _istr.get();
    if (c == '\n')
        ++_lineno;
    return static_cast<char>(c);
}

char Parser::peek() {
    if (_istr.eof())
        error("incorrect Parser::peek() call: EOF found");
    return static_cast<char>(_istr.peek());
}

void Parser::skipSpaces() {
    while (!_istr.eof() && isspace(peek()))
        next();
}

Parser::_Token Parser::nextToken() {
    skipSpaces();
    if (_istr.eof())
        return _Token(TT_NO);
    char firstChar = next();
    while (firstChar == ';') {
	while (next() != '\n' && !_istr.eof()) ;
	skipSpaces();
	if (_istr.eof())
	    return _Token(TT_NO);
        firstChar = next();
    }
    if (firstChar == '(') {
        return _Token(TT_OPEN);
    } else if (firstChar == ')') {
        return _Token(TT_CLOSE);
    } else if (firstChar == '"') {
        std::string res;
        for (;;) {
            if (_istr.eof())
                error("unexpected end-of-file");
            char c = next();
            if (c == '\\')
                res.push_back(getEscape());
            else if (c == '"')
                break;
            else
                res.push_back(c);
        }
        return new String(res);
    } else if (firstChar == '#') {
        if (next() != '\'')
            error("incorrect character literal");
        char c = next();
        if (c == '\\')
            c = getEscape();
        if (next() != '\'')
            error("multicharacter char constant");
        return new Character(c);
    } else if (firstChar == '\'') {
        return _Token(TT_QUOTE);
    } else {
        std::string str(1, firstChar);
        while (!_istr.eof() && !strchr("\n\t\v\r ()\f", peek()))
            str.push_back(next());
        if (Integer::checkString(str))
            return _Token(new Integer(str));
        else if (Double::checkString(str))
            return _Token(new Double(str));
        else if (str == "nil")
            return NullObject::getNullObject();
        else
            return _Token(Symbol::getSymbol(str));
    }
}

Object *Parser::nextExpression() {
    GCProtector prot;
    _Token firstToken = nextToken();
    if (firstToken.getType() == TT_NO) {
        return nullptr;
    } else if (firstToken.getType() == TT_NORMAL) {
        return firstToken.getObject();
    } else if (firstToken.getType() == TT_CLOSE) {
        error("unmatched parenthesis at line %d", _lineno);
    } else if (firstToken.getType() == TT_OPEN) {
        std::vector<std::vector<Object *>> stack;
        stack.push_back(std::vector<Object *>());
        for (;;) {
            _Token tok = nextToken();
            if (tok.getType() == TT_NO) {
                error("unexpected end-of-file");
            } else if (tok.getType() == TT_OPEN) {
                stack.push_back(std::vector<Object *>());
            } else if (tok.getType() == TT_NORMAL) {
                stack.back().push_back(tok.getObject());
            } else if (tok.getType() == TT_CLOSE) {
                Object *newList;
                newList = ConsCell::fromVector(stack.back());
                stack.pop_back();
                if (stack.empty())
                    return newList;
                else
                    stack.back().push_back(newList);
            } else if (tok.getType() == TT_QUOTE) {
                stack.push_back(std::vector<Object *>());
                stack.back().push_back(Symbol::getSymbol("quote"));
                stack.back().push_back(nextExpression());
                Object *newList = ConsCell::fromVector(stack.back());
                stack.pop_back();
                stack.back().push_back(newList);
            } else {
                error("unknown token type: %d", firstToken.getType());
            }
        }
    } else if (firstToken.getType() == TT_QUOTE) {
        std::vector<Object *> lst;
        lst.push_back(Symbol::getSymbol("quote"));
        lst.push_back(nextExpression());
        return ConsCell::fromVector(lst);
    } else {
        error("unknown token type: %d", firstToken.getType());
    }
}

char Parser::getEscape() {
    char firstChar = next();
    if (firstChar == 'n') {
        return '\n';
    } else if (firstChar == 'b') {
        return '\b';
    } else if (firstChar =='r') {
        return '\r';
    } else if (firstChar == 't') {
        return '\t';
    } else if (firstChar == 'f') {
        return '\f';
    } else if (firstChar == 'v') {
        return '\v';
    } else if (firstChar == '\\') {
        return '\\';
    } else if (firstChar == '\'') {
        return '\'';
    } else if (firstChar == '"')  {
        return '"';
    } else if (firstChar == 'x') {
        char s[3];
        s[0] = next();
        s[1] = next();
        s[2] = '\0';
        for (int i = 0; i < 2; ++i)
            if (!isxdigit(s[i]))
                error("incorrect escape sequence at line %d", _lineno);
        unsigned int code;
        sscanf(s, "%x", &code);
        return static_cast<char>(static_cast<unsigned char>(code));
    } else if (firstChar == '0') {
        return '\0';
    } else {
        error("incorrect escape sequence at line %d", _lineno);
    }
}

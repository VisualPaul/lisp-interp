#include "functions.h"
#include "scope.h"
#include <iostream>
#include "parser.h"

namespace functions {
    const std::string EvalFunction::NAME("eval");
    Object *EvalFunction::call(Arguments &args) {
        if (args.positionArgs() != 1)
            argumentNumberError(args, 1);
        return args.getArg(0)->evalute(Scope::global());
    }
  
    const std::string PrintFunction::NAME("print");
    Object *PrintFunction::call(Arguments &args) {
	if (args.positionArgs() == 0)
	    argumentNumberError(args, 1);
	for (int i = 0; i < args.positionArgs(); ++i) {
	    if (i)
		std::cout << ' ';
	    args.getArg(i)->print(std::cout);
	}
	return NullObject::null;
    }

    const std::string ReadFunction::NAME("read");
    Object *ReadFunction::call(Arguments &args) {
	if (args.positionArgs() != 0)
	    argumentNumberError(args, 0);
	Parser par(std::cin);
	return par.nextExpression();
    }

    const std::string PrintLineFunction::NAME("print-line");
    Object *PrintLineFunction::call(Arguments &args) {
	if (args.positionArgs() == 0)
	    argumentNumberError(args, 1);
	for (int i = 0; i < args.positionArgs(); ++i) {
	    if (i)
		std::cout << ' ';
	    args.getArg(i)->print(std::cout);
	}
	std::cout << '\n';
	return NullObject::null;
    }
    
    
    void init(Scope *scope) {
        numeric_init(scope);
        list_init(scope);
        _add(EvalFunction::object, scope);
	_add(PrintFunction::object, scope);
	_add(ReadFunction::object, scope);
	_add(PrintLineFunction::object, scope);
    }
}


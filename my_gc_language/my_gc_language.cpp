// my_gc_language.cpp : Defines the entry point for the application.
//

#include "my_gc_language.h"
#include "gc.h"
#include "variable.h"
#include "expression.h"
#include "function.h"
#include "interpreter.h"

using namespace MaiaLang;

int main()
{
	GC::printInfo();

	Interpreter interpreter;

	try {
		interpreter.execute("test.maia", "let string c = \"Hello, World! Maia lang\";\nlet int d = 5;");
	}
	catch (const std::runtime_error &err) {
		std::cerr << err.what() << '\n';
		return -1;
	}

	GC::collect();

	GC::printInfo();

	return 0;
}


/*
* let string a;
* a = "Hello, World!";
* println(a);
*/
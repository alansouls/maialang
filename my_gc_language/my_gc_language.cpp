#include "my_gc_language.h"
#include "gc.h"
#include "interpreter.h"
#include "lexer.h"

using namespace MaiaLang;

static auto tokenTypeStr(Lexer::TokenType tokenType) -> std::string
{
	switch (tokenType) {
	case Lexer::TokenType::Let:
		return "let";
	case Lexer::TokenType::Identitifer:
		return "identifier";
	case Lexer::TokenType::Assignment:
		return "assignment";
	case Lexer::TokenType::Number:
		return "number";
	case Lexer::TokenType::StringLiteral:
		return "string_literal";
	case Lexer::TokenType::ParenthesesOpen:
		return "parentheses_open";
	case Lexer::TokenType::ParenthesesClose:
		return "parentheses_close";
	case Lexer::TokenType::Comma:
		return "comma";
	case Lexer::TokenType::SemiCollon:
		return "semicolon";
	case Lexer::TokenType::Plus:
		return "plus";
	case Lexer::TokenType::Minus:
		return "minus";
	case Lexer::TokenType::Star:
		return "star";
	case Lexer::TokenType::Slash:
		return "slash";
	default:
		throw std::runtime_error("Unknown token type");
	}
}


int main()
{
	GC::printInfo();

	Interpreter interpreter;

	(void)interpreter;

	try {
		//interpreter.execute("test.maia", "let string c = \"Hello, World! Maia lang\";\nlet int d = 5;\nprintln(\"Hello, World - with MaiaLang!\");");
		//interpreter.execute("test.maia", "println(\"Hello World - with MaiaLang!\");");
		Lexer lexer("let string c = \"Hello, World! Maia lang\";\nprintln(\"Hello, World - with MaiaLang!\");\nlet int a = 10;");

		for (auto& token : lexer.tokens()) {
			std::cout << "Token: " << token.token << " " << tokenTypeStr(token.type) << "\n";
		}
	}
	catch (const std::runtime_error& err) {
		std::cerr << err.what() << '\n';
		return -1;
	}

	GC::collect();

	GC::printInfo();
}

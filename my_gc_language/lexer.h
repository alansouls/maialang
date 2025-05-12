#pragma once
#include <vector>
#include <string>

namespace MaiaLang
{
	class Lexer 
	{
	public:
		enum TokenType 
		{
			Let,
			Identitifer,
			Assignment,
			Number,
			StringLiteral,
			ParenthesesOpen,
			ParenthesesClose,
			Comma,
			SemiCollon,
			Plus,
			Minus,
			Star,
			Slash
		};

		struct Token
		{
			std::string token;
			TokenType type;
		};

		Lexer(const std::string& code);

		auto tokens() -> const std::vector<Token>&;

	private:
		std::vector<Token> m_tokens;

		struct CapturingGroup
		{
			size_t start;
			size_t count;
		};

		void init(const std::string& code);

		void processCapturingGroup(size_t newIndex, CapturingGroup& group, const std::string& code);
		void addOperatorToken(const char& operatorChar);
		void addParenthesesToken(const char& parenthesesChar);
	};
}
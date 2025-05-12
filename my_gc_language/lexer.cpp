#include "lexer.h"
#include <stdexcept>

namespace MaiaLang
{
	static bool isWhiteSpace(const char& c)
	{
		return c == ' ' || c == '\n' || c == '\r' || c == '\t';
	}

	static bool isOperator(const char& c)
	{
		return c == '+' || c == '-' || c == '/' || c == '*';
	}

	static bool isParentheses(const char& c) {
		return c == '(' || c == ')';
	}

	static bool isComma(const char& c) {
		return c == ',';
	}

	static bool isSemicolon(const char& c) {
		return c == ';';
	}

	static bool isEqualsSign(const char& c) {
		return c == '=';
	}

	static bool isDigit(const char& c) {
		return c >= '0' && c <= '9';
	}

	Lexer::Lexer(const std::string& code) : m_tokens()
	{
		init(code);
	}

	auto Lexer::tokens() -> const std::vector<Token>&
	{
		return m_tokens;
	}

	void Lexer::init(const std::string& code)
	{
		CapturingGroup group{
			.start = 0,
			.count = 0
		};

		for (size_t i = 0; i < code.size(); ++i) {
			const char& c = code[i];

			const char& firstCharAtGroup = code[group.start];

			auto isCapturingStringLiteral = firstCharAtGroup == '\"';

			if (isCapturingStringLiteral) {
				group.count++;
				if (c == '\"') {
					processCapturingGroup(i + 1, group, code);
					++i;
				}
				continue;
			}

			if (isWhiteSpace(c) || isOperator(c) || isParentheses(c) || isComma(c) || isSemicolon(c) || isEqualsSign(c)
				|| isWhiteSpace(firstCharAtGroup) || isOperator(firstCharAtGroup) || isParentheses(firstCharAtGroup) || isComma(firstCharAtGroup)
				|| isSemicolon(firstCharAtGroup) || isEqualsSign(firstCharAtGroup)) {
				processCapturingGroup(i, group, code);
				continue;
			}

			group.count++;
		}

		processCapturingGroup(0, group, code);
	}

	void Lexer::processCapturingGroup(size_t newIndex, CapturingGroup& group, const std::string& code)
	{
		if (const char& c = code[group.start]; group.count == 1) {
			if (isWhiteSpace(c)) {
				//do nothing
			}
			else if (isOperator(c)) {
				addOperatorToken(c);
			}
			else if (isParentheses(c)) {
				addParenthesesToken(c);
			}
			else if (isComma(c)) {
				m_tokens.push_back({ "", TokenType::Comma });
			}
			else if (isSemicolon(c)) {
				m_tokens.push_back({ "", TokenType::SemiCollon });
			}
			else if (isEqualsSign(c)) {
				m_tokens.push_back({ "", TokenType::Assignment });
			}
			else {
				m_tokens.push_back({ std::string(1, c), TokenType::Identitifer });
			}
		}
		else if (group.count > 0) {
			auto tokenValue = code.substr(group.start, group.count);

			if (tokenValue == "let") {
				m_tokens.push_back({ "", TokenType::Let });
			}
			else if (tokenValue.starts_with('\"')) {
				m_tokens.push_back({ tokenValue, TokenType::StringLiteral });
			}
			else if (isDigit(tokenValue[0])) {
				m_tokens.push_back({ tokenValue, TokenType::Number });
			}
			else {
				m_tokens.push_back({ code.substr(group.start, group.count), TokenType::Identitifer });
			}
		}

		group.start = newIndex;
		group.count = 1;
	}

	void Lexer::addOperatorToken(const char& operatorChar)
	{
		switch (operatorChar) {
		case '+':
			m_tokens.push_back({ "", TokenType::Plus });
			break;
		case '-':
			m_tokens.push_back({ "",TokenType::Minus });
			break;
		case '*':
			m_tokens.push_back({ "",TokenType::Star });
			break;
		case '/':
			m_tokens.push_back({ "",TokenType::Slash });
			break;
		default:
			throw std::runtime_error("Unknown operator");
		}
	}

	void Lexer::addParenthesesToken(const char& parenthesesChar)
	{
		if (parenthesesChar == '(') {
			m_tokens.push_back({ "",TokenType::ParenthesesOpen });
		}
		else {
			m_tokens.push_back({ "",TokenType::ParenthesesClose });
		}
	}
}

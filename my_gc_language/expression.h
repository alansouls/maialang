
#pragma once
#include "gc.h"
#include <functional>
#include <optional>
#include <stdexcept>

namespace MaiaLang
{
	class Variable;
	class Scope;
	class Expression;

	struct FunctionCallParameters
	{
		std::string functionName;
		std::vector<std::shared_ptr<Expression>> parameterExpressions;
	};

	struct VariableAssignmentParameters
	{
		std::string variableName;
		std::shared_ptr<Expression> expression;
	};

	struct VariableDeclarationParameters
	{
		std::string typeName;
		std::string variableName;
		std::optional<VariableAssignmentParameters> assignment;
	};

	enum OperationType
	{
		VariableDeclaration,
		VariableAssignment,
		ConstValue,
		FunctionCall
	};

	class ExpressionParameters
	{
	private:
		union ExpressionParametersUnion
		{
			FunctionCallParameters* functionCall;
			VariableDeclarationParameters* variableDeclaration;
			VariableAssignmentParameters* variableAssignment;
			std::string* constValue;
		};

	public:
		ExpressionParameters(OperationType type) : m_type(type), m_value{ nullptr }
		{
		}

		ExpressionParameters(const std::string& constValue) : ExpressionParameters(OperationType::ConstValue)
		{
			m_value.constValue = new std::string(constValue);
		}

		ExpressionParameters(const VariableDeclarationParameters& variableDeclarationParameters)
			: ExpressionParameters(OperationType::VariableDeclaration)
		{
			m_value.variableDeclaration = new VariableDeclarationParameters(variableDeclarationParameters);
		}

		ExpressionParameters(const VariableAssignmentParameters& variableAssignmentParameters)
			: ExpressionParameters(OperationType::VariableAssignment)
		{
			m_value.variableAssignment = new VariableAssignmentParameters(variableAssignmentParameters);
		}

		ExpressionParameters(const FunctionCallParameters& parameters)
			: ExpressionParameters(OperationType::FunctionCall)
		{
			m_value.functionCall = new FunctionCallParameters(parameters);
		}

		ExpressionParameters(const ExpressionParameters& parameters) : ExpressionParameters(parameters.m_type)
		{
			switch (m_type)
			{
			case MaiaLang::VariableDeclaration:
				m_value.variableDeclaration = new VariableDeclarationParameters(parameters.getVariableDeclaration());
				break;
			case MaiaLang::VariableAssignment:
				m_value.variableAssignment = new VariableAssignmentParameters(parameters.getVariableAssignmentParameters());
				break;
			case MaiaLang::ConstValue:
				m_value.constValue = new std::string(parameters.getConstValue());
				break;
			case MaiaLang::FunctionCall:
				m_value.functionCall = new FunctionCallParameters(parameters.getFunctionCallParameters());
				break;
			default:
				break;
			}
		}

		~ExpressionParameters()
		{
			switch (m_type)
			{
			case MaiaLang::VariableDeclaration:
				delete m_value.variableDeclaration;
				break;
			case MaiaLang::VariableAssignment:
				delete m_value.variableAssignment;
				break;
			case MaiaLang::ConstValue:
				delete m_value.constValue;
				break;
			default:
				break;
			}
		}

		auto getConstValue() const -> const std::string& {
			if (m_type != OperationType::ConstValue) {
				throw std::runtime_error("Invalid type");
			}

			return *m_value.constValue;
		}

		auto getVariableDeclaration() const -> const VariableDeclarationParameters& {
			if (m_type != OperationType::VariableDeclaration) {
				throw std::runtime_error("Invalid type");
			}

			return *m_value.variableDeclaration;
		}

		auto getVariableAssignmentParameters() const -> const VariableAssignmentParameters& {
			if (m_type != OperationType::VariableAssignment) {
				throw std::runtime_error("Invalid type");
			}

			return *m_value.variableAssignment;
		}

		auto getFunctionCallParameters() const -> const FunctionCallParameters& {
			if (m_type != OperationType::FunctionCall) {
				throw std::runtime_error("Invalid type");
			}

			return *m_value.functionCall;
		}

		auto type() const -> OperationType {
			return m_type;
		}

	private:
		OperationType m_type;
		ExpressionParametersUnion m_value;
	};

	struct ExpressionValue
	{
		MemoryAllocation memoryValue;
	};

	class Expression
	{
	public:
		Expression(const ExpressionParameters& parameters);

		auto evaluate(Scope &scope) const->std::optional<ExpressionValue>;
	private:
		ExpressionParameters m_parameters;

		static auto declareVariable(Scope &scope, const VariableDeclarationParameters& parameters) -> std::weak_ptr<Variable>;
		static auto assignVariable(Scope& scope, const VariableAssignmentParameters& parameters) -> MemoryAllocation;
		static auto callFunction(Scope& scope, const FunctionCallParameters& parameters) -> std::optional<ExpressionValue>;
	};
}
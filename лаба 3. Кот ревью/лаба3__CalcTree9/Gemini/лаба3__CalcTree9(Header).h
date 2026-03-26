#pragma once

#include <string>
#include <vector>
#include <fstream>

/** Тип узла в дереве выражений */
enum class ENodeType
{
	Operator,
	Operand,
	Variable
};

/** Узел дерева выражения */
struct FExpressionNode
{
	ENodeType Type;
	int Value; // Используется для чисел
	char Operator; // Используется для операций
	FExpressionNode* Left;
	FExpressionNode* Right;

	FExpressionNode(ENodeType InType, int InValue = 0, char InOperator = '\0')
		: Type(InType)
		, Value(InValue)
		, Operator(InOperator)
		, Left(nullptr)
		, Right(nullptr)
	{}

	~FExpressionNode()
	{
		delete Left;
		delete Right;
	}
};

/** Класс для работы с деревом выражений */
class FExpressionTree
{
public:
	FExpressionTree();
	~FExpressionTree();

	/** Строит дерево из инфиксной строки */
	bool BuildFromInfix(const std::string& InExpression);

	/** Вычисляет значение выражения */
	int Evaluate(int XValue) const;

	/** Трансформирует дерево по правилу A + x -> x + A */
	void Transform();

	/** Методы вывода в разных формах */
	void PrintPrefix(std::ofstream& OutFile) const;
	void PrintPostfix(std::ofstream& OutFile) const;
	void PrintInfix(std::ofstream& OutFile) const;

private:
	FExpressionNode* Root;

	/** Вспомогательные методы */
	int GetPriority(char Op) const;
	int Power(int Base, int Exp) const;
	FExpressionNode* BuildTreeFromPostfix(const std::vector<std::string>& PostfixTokens);

	int EvaluateRecursive(FExpressionNode* Node, int XValue) const;
	void TransformRecursive(FExpressionNode* Node);

	void PrintPrefixRecursive(FExpressionNode* Node, std::ofstream& OutFile) const;
	void PrintPostfixRecursive(FExpressionNode* Node, std::ofstream& OutFile) const;
	void PrintInfixRecursive(FExpressionNode* Node, std::ofstream& OutFile) const;

	bool IsOperator(char C) const;
	bool IsDigit(char C) const;
};
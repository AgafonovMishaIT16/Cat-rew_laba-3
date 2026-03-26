#include "лаба3__CalcTree9(Header).h"
#include <stack>
#include <cmath>
#include <stdexcept>

FExpressionTree::FExpressionTree() : Root(nullptr) {}

FExpressionTree::~FExpressionTree() { delete Root; }

bool FExpressionTree::IsOperator(char C) const
{
	return C == '+' || C == '-' || C == '*' || C == '/' || C == '%' || C == '^';
}

bool FExpressionTree::IsDigit(char C) const
{
	return C >= '0' && C <= '9';
}

int FExpressionTree::GetPriority(char Op) const
{
	if (Op == '+' || Op == '-') return 1;
	if (Op == '*' || Op == '/' || Op == '%') return 2;
	if (Op == '^') return 3;
	return 0;
}

int FExpressionTree::Power(int Base, int Exp) const
{
	int Result = 1;
	for (int i = 0; i < Exp; ++i) Result *= Base;
	return Result;
}

bool FExpressionTree::BuildFromInfix(const std::string& InExpression)
{
	delete Root; // Очищаем старое дерево, если оно было
	Root = nullptr;

	std::vector<std::string> Postfix;
	std::stack<char> OperatorStack;

	for (size_t i = 0; i < InExpression.length(); ++i)
	{
		char C = InExpression[i];
		// Безопасная замена isspace(C)
		if (C == ' ' || C == '\t' || C == '\n' || C == '\r') continue;

		if (IsDigit(C))
		{
			std::string Num;
			while (i < InExpression.length() && IsDigit(InExpression[i]))
			{
				Num += InExpression[i++];
			}
			--i;

			try
			{
				int Val = std::stoi(Num);
				if (Val < 1 || Val > 30) return false; // Валидация [1; 30]
			}
			catch (...)
			{
				return false; // Защита от слишком длинных чисел (std::out_of_range)
			}
			Postfix.push_back(Num);
		}
		else if (C == 'x')
		{
			Postfix.push_back("x");
		}
		else if (C == '(')
		{
			OperatorStack.push(C);
		}
		else if (C == ')')
		{
			while (!OperatorStack.empty() && OperatorStack.top() != '(')
			{
				Postfix.push_back(std::string(1, OperatorStack.top()));
				OperatorStack.pop();
			}
			if (!OperatorStack.empty()) OperatorStack.pop();
		}
		else if (IsOperator(C))
		{
			while (!OperatorStack.empty() && GetPriority(OperatorStack.top()) >= GetPriority(C))
			{
				Postfix.push_back(std::string(1, OperatorStack.top()));
				OperatorStack.pop();
			}
			OperatorStack.push(C);
		}
		else return false; // Недопустимый символ
	}

	while (!OperatorStack.empty())
	{
		// Если остались скобки, значит баланс нарушен
		if (OperatorStack.top() == '(') return false;
		Postfix.push_back(std::string(1, OperatorStack.top()));
		OperatorStack.pop();
	}

	Root = BuildTreeFromPostfix(Postfix);
	return Root != nullptr;
}

FExpressionNode* FExpressionTree::BuildTreeFromPostfix(const std::vector<std::string>& PostfixTokens)
{
	std::stack<FExpressionNode*> NodeStack;
	for (const std::string& Token : PostfixTokens)
	{
		if (IsDigit(Token[0]))
		{
			NodeStack.push(new FExpressionNode(ENodeType::Operand, std::stoi(Token)));
		}
		else if (Token == "x")
		{
			NodeStack.push(new FExpressionNode(ENodeType::Variable));
		}
		else
		{
			FExpressionNode* Node = new FExpressionNode(ENodeType::Operator, 0, Token[0]);
			if (NodeStack.size() < 2)
			{
				// Безопасная очистка памяти при неверном дереве
				delete Node;
				while (!NodeStack.empty()) { delete NodeStack.top(); NodeStack.pop(); }
				return nullptr;
			}
			Node->Right = NodeStack.top(); NodeStack.pop();
			Node->Left = NodeStack.top(); NodeStack.pop();
			NodeStack.push(Node);
		}
	}

	if (NodeStack.size() != 1)
	{
		// Если операндов больше чем нужно
		while (!NodeStack.empty()) { delete NodeStack.top(); NodeStack.pop(); }
		return nullptr;
	}

	return NodeStack.top();
}

int FExpressionTree::Evaluate(int XValue) const
{
	return EvaluateRecursive(Root, XValue);
}

int FExpressionTree::EvaluateRecursive(FExpressionNode* Node, int XValue) const
{
	if (!Node) return 0;
	if (Node->Type == ENodeType::Operand) return Node->Value;
	if (Node->Type == ENodeType::Variable) return XValue;

	int L = EvaluateRecursive(Node->Left, XValue);
	int R = EvaluateRecursive(Node->Right, XValue);

	switch (Node->Operator)
	{
	case '+': return L + R;
	case '-': return L - R;
	case '*': return L * R;
	case '/': return (R != 0) ? L / R : 0;
	case '%': return (R != 0) ? L % R : 0;
	case '^': return Power(L, R);
	default: return 0;
	}
}

void FExpressionTree::Transform() { TransformRecursive(Root); }

void FExpressionTree::TransformRecursive(FExpressionNode* Node)
{
	if (!Node) return;
	TransformRecursive(Node->Left);
	TransformRecursive(Node->Right);

	// Безопасная проверка: A + x -> x + A
	if (Node->Type == ENodeType::Operator && Node->Operator == '+')
	{
		if (Node->Right && Node->Right->Type == ENodeType::Variable &&
			Node->Left && Node->Left->Type != ENodeType::Variable)
		{
			FExpressionNode* Temp = Node->Left;
			Node->Left = Node->Right;
			Node->Right = Temp;
		}
	}
}

void FExpressionTree::PrintPrefix(std::ofstream& OutFile) const { PrintPrefixRecursive(Root, OutFile); OutFile << "\n"; }
void FExpressionTree::PrintPostfix(std::ofstream& OutFile) const { PrintPostfixRecursive(Root, OutFile); OutFile << "\n"; }
void FExpressionTree::PrintInfix(std::ofstream& OutFile) const { PrintInfixRecursive(Root, OutFile); OutFile << "\n"; }

void FExpressionTree::PrintPrefixRecursive(FExpressionNode* Node, std::ofstream& OutFile) const
{
	if (!Node) return;
	if (Node->Type == ENodeType::Operand) OutFile << Node->Value << " ";
	else if (Node->Type == ENodeType::Variable) OutFile << "x ";
	else OutFile << Node->Operator << " ";
	PrintPrefixRecursive(Node->Left, OutFile);
	PrintPrefixRecursive(Node->Right, OutFile);
}

void FExpressionTree::PrintPostfixRecursive(FExpressionNode* Node, std::ofstream& OutFile) const
{
	if (!Node) return;
	PrintPostfixRecursive(Node->Left, OutFile);
	PrintPostfixRecursive(Node->Right, OutFile);
	if (Node->Type == ENodeType::Operand) OutFile << Node->Value << " ";
	else if (Node->Type == ENodeType::Variable) OutFile << "x ";
	else OutFile << Node->Operator << " ";
}

void FExpressionTree::PrintInfixRecursive(FExpressionNode* Node, std::ofstream& OutFile) const
{
	if (!Node) return;
	if (Node->Type == ENodeType::Operator) OutFile << "(";
	PrintInfixRecursive(Node->Left, OutFile);

	if (Node->Type == ENodeType::Operand) OutFile << Node->Value;
	else if (Node->Type == ENodeType::Variable) OutFile << "x";
	else OutFile << " " << Node->Operator << " ";

	PrintInfixRecursive(Node->Right, OutFile);
	if (Node->Type == ENodeType::Operator) OutFile << ")";
}
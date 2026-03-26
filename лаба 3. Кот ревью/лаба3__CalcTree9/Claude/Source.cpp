#include "Header.h"

#include <algorithm>   // std::swap
#include <cassert>
#include <cctype>
#include <stdexcept>
#include <string>
#include <utility>


/*  Свободные вспомогательные функции(внутреннее связывание) */

/**
 * Целочисленное возведение в степень. Exp должно быть >= 0 согласно условию задачи.
 */
static int IntPow(int Base, int Exp)
{
    if (Exp < 0)
    {
        throw std::runtime_error("Показатель степени должен быть неотрицательным целым числом.");
    }

    int Result = 1;
    for (int Idx = 0; Idx < Exp; ++Idx)
    {
        Result *= Base;
    }
    return Result;
}


bool ValidateRawInput(const std::string& Content, std::string& OutError)
{
    for (const char Ch : Content)
    {
        const bool bAllowed =
            std::isdigit(static_cast<unsigned char>(Ch)) ||
            Ch == 'x' ||
            Ch == '+' || Ch == '-' ||
            Ch == '*' || Ch == '/' ||
            Ch == '%' || Ch == '^' ||
            Ch == '(' || Ch == ')' ||
            std::isspace(static_cast<unsigned char>(Ch));

        if (!bAllowed)
        {
            OutError = "Недопустимый символ во вводе: '";
            OutError += Ch;
            OutError += "'. Разрешены: цифры, x, +, -, *, /, %, ^, (, ), пробелы.";
            return false;
        }
    }
    return true;
}
/*  FTreeNode — конструкторы */

FTreeNode::FTreeNode(int InValue)
    : NodeType(ENodeType::Number)
    , Value(InValue)
    , Op('\0')
    , Left(nullptr)
    , Right(nullptr)
{}

FTreeNode::FTreeNode()
    : NodeType(ENodeType::Variable)
    , Value(0)
    , Op('\0')
    , Left(nullptr)
    , Right(nullptr)
{}

FTreeNode::FTreeNode(char InOp, FTreeNode* InLeft, FTreeNode* InRight)
    : NodeType(ENodeType::BinaryOperator)
    , Value(0)
    , Op(InOp)
    , Left(InLeft)
    , Right(InRight)
{}


/*  FExpressionTree — конструктор / деструктор */


FExpressionTree::FExpressionTree()
    : Root(nullptr)
    , TokenPos(0)
{}

FExpressionTree::~FExpressionTree()
{
    DeleteSubtree(Root);
}

void FExpressionTree::DeleteSubtree(FTreeNode* Node)
{
    if (!Node)
    {
        return;
    }
    DeleteSubtree(Node->Left);
    DeleteSubtree(Node->Right);
    delete Node;
}

bool FExpressionTree::TokenizeExpression(const std::string& InExpression)
{
    Tokens.clear();
    TokenPos = 0;

    const size_t ExprLen = InExpression.size();
    size_t       Idx = 0;

    while (Idx < ExprLen)
    {
        const unsigned char Ch = static_cast<unsigned char>(InExpression[Idx]);

        // Пропуск пробелов
        if (std::isspace(Ch))
        {
            ++Idx;
            continue;
        }

        // Целочисленный литерал
        if (std::isdigit(Ch))
        {
            std::string NumStr;
            while (Idx < ExprLen && std::isdigit(static_cast<unsigned char>(InExpression[Idx])))
            {
                NumStr += InExpression[Idx++];
            }

            const int NumVal = std::stoi(NumStr);
            if (NumVal < 1 || NumVal > 30)
            {
                LastError = "Число " + NumStr;
                LastError += " выходит за допустимый диапазон [1, 30].";
                return false;
            }

            Tokens.push_back(NumStr);
            continue;
        }

        // Переменная
        if (InExpression[Idx] == 'x')
        {
            Tokens.emplace_back("x");
            ++Idx;
            continue;
        }

        // Оператор или скобка
        const char OpCh = InExpression[Idx];
        if (OpCh == '+' || OpCh == '-' ||
            OpCh == '*' || OpCh == '/' ||
            OpCh == '%' || OpCh == '^' ||
            OpCh == '(' || OpCh == ')')
        {
            Tokens.emplace_back(1, OpCh);
            ++Idx;
            continue;
        }

        // Все остальное недопустимо
        LastError = "Неожиданный символ '";
        LastError += InExpression[Idx];
        LastError += "' найден во время токенизации.";
        return false;
    }

    return true;
}


bool FExpressionTree::Build(const std::string& InExpression)
{

    DeleteSubtree(Root);
    Root = nullptr;
    LastError.clear();


    if (!TokenizeExpression(InExpression))
    {
        return false;
    }

    if (Tokens.empty())
    {
        LastError = "Выражение пустое.";
        return false;
    }


    try
    {
        Root = ParseAddSub();
    }
    catch (const std::runtime_error& Ex)
    {
        LastError = Ex.what();
        DeleteSubtree(Root);
        Root = nullptr;
        return false;
    }


    if (TokenPos != Tokens.size())
    {
        LastError = "Неожиданный токен '";
        LastError += Tokens[TokenPos];
        LastError += "' — выражение разобрано не полностью.";
        DeleteSubtree(Root);
        Root = nullptr;
        return false;
    }

    return true;
}



FTreeNode* FExpressionTree::ParseAddSub()
{
    FTreeNode* LeftNode = ParseMulDivMod();

    while (TokenPos < Tokens.size() &&
        (Tokens[TokenPos] == "+" || Tokens[TokenPos] == "-"))
    {
        const char Op = Tokens[TokenPos++][0];
        FTreeNode* RightNode = ParseMulDivMod();
        LeftNode = new FTreeNode(Op, LeftNode, RightNode);
    }

    return LeftNode;
}

FTreeNode* FExpressionTree::ParseMulDivMod()
{
    FTreeNode* LeftNode = ParsePower();

    while (TokenPos < Tokens.size() &&
        (Tokens[TokenPos] == "*" ||
            Tokens[TokenPos] == "/" ||
            Tokens[TokenPos] == "%"))
    {
        const char Op = Tokens[TokenPos++][0];
        FTreeNode* RightNode = ParsePower();
        LeftNode = new FTreeNode(Op, LeftNode, RightNode);
    }

    return LeftNode;
}

FTreeNode* FExpressionTree::ParsePower()
{
    FTreeNode* LeftNode = ParsePrimary();

    while (TokenPos < Tokens.size() && Tokens[TokenPos] == "^")
    {
        const char Op = Tokens[TokenPos++][0];
        FTreeNode* RightNode = ParsePrimary();
        LeftNode = new FTreeNode(Op, LeftNode, RightNode);
    }

    return LeftNode;
}

FTreeNode* FExpressionTree::ParsePrimary()
{
    if (TokenPos >= Tokens.size())
    {
        throw std::runtime_error("Неожиданный конец выражения.");
    }

    const std::string& Token = Tokens[TokenPos];

    /* Подвыражение в скобках */
    if (Token == "(")
    {
        ++TokenPos; // потребляем '('
        FTreeNode* Node = ParseAddSub();

        if (TokenPos >= Tokens.size() || Tokens[TokenPos] != ")")
        {
            throw std::runtime_error("Отсутствует закрывающая скобка ')'.");
        }

        ++TokenPos; // потребляем ')'
        return Node;
    }

    // Целочисленный литерал
    if (!Token.empty() && std::isdigit(static_cast<unsigned char>(Token[0])))
    {
        ++TokenPos;
        return new FTreeNode(std::stoi(Token));
    }

    // Переменная
    if (Token == "x")
    {
        ++TokenPos;
        return new FTreeNode(); // Узел переменной
    }

    throw std::runtime_error("Неожиданный токен: '" + Token + "'.");
}



int FExpressionTree::Evaluate(int XValue) const
{
    if (!Root)
    {
        throw std::runtime_error("Дерево пустое; вызовите Build() перед Evaluate().");
    }
    return EvaluateNode(Root, XValue);
}

int FExpressionTree::EvaluateNode(const FTreeNode* Node, int XValue) const
{
    assert(Node != nullptr);

    switch (Node->NodeType)
    {
    case ENodeType::Number:
        return Node->Value;

    case ENodeType::Variable:
        return XValue;

    case ENodeType::BinaryOperator:
    {
        const int L = EvaluateNode(Node->Left, XValue);
        const int R = EvaluateNode(Node->Right, XValue);

        switch (Node->Op)
        {
        case '+': return L + R;
        case '-': return L - R;
        case '*': return L * R;

        case '/':
            if (R == 0)
            {
                throw std::runtime_error("Целочисленное деление на ноль.");
            }
            return L / R;

        case '%':
            if (R == 0)
            {
                throw std::runtime_error("Взятие остатка от деления на ноль.");
            }
            return L % R;

        case '^':
            return IntPow(L, R);

        default:
            throw std::runtime_error(
                std::string("Неизвестный оператор '") + Node->Op + "'.");
        }
    }
    } // switch (Node->NodeType)

    throw std::runtime_error("При вычислении встречен неизвестный тип узла.");
}



void FExpressionTree::TransformAddX()
{
    TransformNode(Root);
}

void FExpressionTree::TransformNode(FTreeNode* Node)
{
    if (!Node)
    {
        return;
    }

    // Преобразуем потомков до проверки этого узла.
    TransformNode(Node->Left);
    TransformNode(Node->Right);

    // Сопоставление с шаблоном:  (Node->Left) + x
    const bool bIsAddOp = (Node->NodeType == ENodeType::BinaryOperator) && (Node->Op == '+');
    const bool bRightIsX = (Node->Right != nullptr) && (Node->Right->NodeType == ENodeType::Variable);

    if (bIsAddOp && bRightIsX)
    {
        std::swap(Node->Left, Node->Right);
    }
}


std::string FExpressionTree::ToPrefixString()  const { return NodeToPrefix(Root); }
std::string FExpressionTree::ToPostfixString() const { return NodeToPostfix(Root); }
std::string FExpressionTree::ToInfixString()   const { return NodeToInfix(Root); }

std::string FExpressionTree::NodeToPrefix(const FTreeNode* Node) const
{
    if (!Node) { return {}; }

    switch (Node->NodeType)
    {
    case ENodeType::Number:
        return std::to_string(Node->Value);

    case ENodeType::Variable:
        return "x";

    case ENodeType::BinaryOperator:
        return std::string(1, Node->Op)
            + ' ' + NodeToPrefix(Node->Left)
            + ' ' + NodeToPrefix(Node->Right);
    }
    return {};
}

std::string FExpressionTree::NodeToPostfix(const FTreeNode* Node) const
{
    if (!Node) { return {}; }

    switch (Node->NodeType)
    {
    case ENodeType::Number:
        return std::to_string(Node->Value);

    case ENodeType::Variable:
        return "x";

    case ENodeType::BinaryOperator:
        return NodeToPostfix(Node->Left)
            + ' ' + NodeToPostfix(Node->Right)
            + ' ' + std::string(1, Node->Op);
    }
    return {};
}

std::string FExpressionTree::NodeToInfix(const FTreeNode* Node) const
{
    if (!Node) { return {}; }

    switch (Node->NodeType)
    {
    case ENodeType::Number:
        return std::to_string(Node->Value);

    case ENodeType::Variable:
        return "x";

    case ENodeType::BinaryOperator:
        return '(' + NodeToInfix(Node->Left)
            + ' ' + std::string(1, Node->Op)
            + ' ' + NodeToInfix(Node->Right) + ')';
    }
    return {};
}

const std::string& FExpressionTree::GetLastError() const
{
    return LastError;
}
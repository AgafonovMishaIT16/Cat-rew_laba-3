#include <fstream>
#include <iostream>
#include "лаба3__CTGPT.Header.h"

int main()
{
    std::ifstream InFile("Текст1.txt");
    std::ofstream OutFile("Текст2.txt");

    if (!InFile.is_open())
    {
        std::cout << "Error opening FN1\n";
        return 1;
    }

    std::string Expression;
    std::getline(InFile, Expression);

    int X;
    InFile >> X;

    FExpressionTree Tree;

    if (!Tree.BuildFromInfix(Expression))
    {
        OutFile << "Invalid expression\n";
        return 1;
    }

    int Result = Tree.Evaluate(X);
    OutFile << "Result: " << Result << "\n";

    Tree.Transform();

    OutFile << "Prefix: " << Tree.ToPrefix() << "\n";
    OutFile << "Postfix: " << Tree.ToPostfix() << "\n";
    OutFile << "Infix: " << Tree.ToInfix() << "\n";

    return 0;
}
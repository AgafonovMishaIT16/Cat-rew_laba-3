#include "лаба3__CalcTree9(Header).h"
#include <iostream>
#include <string>

int main()
{
	std::ifstream InputFile("FN1.txt");
	if (!InputFile.is_open())
	{
		std::cerr << "Error: Cannot open FN1.txt" << std::endl;
		return 1;
	}

	std::string Expression;
	std::getline(InputFile, Expression);
	InputFile.close();

	FExpressionTree Tree;
	if (!Tree.BuildFromInfix(Expression))
	{
		std::cerr << "Error: Invalid expression or numbers out of range [1, 30]" << std::endl;
		return 1;
	}

	int XValue = 0;
	bool bIsValid = false;
	std::string InputLine;

	std::cout << "Enter value for x: ";

	// Строгая валидация ввода (только целые числа)
	while (!bIsValid)
	{
		std::getline(std::cin, InputLine);

		// Отрезаем возможные случайные пробелы по краям строки
		size_t First = InputLine.find_first_not_of(" \t\r\n");
		if (First == std::string::npos) continue; // Если нажали просто Enter

		size_t Last = InputLine.find_last_not_of(" \t\r\n");
		std::string CleanInput = InputLine.substr(First, Last - First + 1);

		// Проверяем, что внутри остались строго только цифры (и возможный знак в начале)
		bool bIsNumber = true;
		size_t StartIdx = 0;
		if (CleanInput[0] == '-' || CleanInput[0] == '+')
		{
			StartIdx = 1;
			if (CleanInput.length() == 1) bIsNumber = false; // Ошибка, если ввели только один минус или плюс
		}

		for (size_t i = StartIdx; i < CleanInput.length(); ++i)
		{
			// Если символ не является цифрой
			if (CleanInput[i] < '0' || CleanInput[i] > '9')
			{
				bIsNumber = false;
				break;
			}
		}

		if (bIsNumber)
		{
			try
			{
				XValue = std::stoi(CleanInput);
				bIsValid = true; // Выходим из цикла, если всё успешно
			}
			catch (...)
			{
				// Защита на случай, если ввели гигантское число, которое не влезет в int
				std::cout << "Error: Number is out of range. Enter value for x: ";
			}
		}
		else
		{
			std::cout << "Error: Invalid input. ONLY numbers are allowed. Enter value for x: ";
		}
	}

	std::ofstream OutputFile("FN2.txt");
	if (!OutputFile.is_open())
	{
		std::cerr << "Error: Cannot open FN2.txt" << std::endl;
		return 1;
	}

	// 1. Вычисляем значение и выводим
	int Result = Tree.Evaluate(XValue);
	OutputFile << "Evaluation result: " << Result << "\n\n";

	// 2. Трансформируем дерево
	Tree.Transform();

	// 3. Выводим формы
	OutputFile << "Transformed tree forms:\n";
	OutputFile << "Prefix: ";
	Tree.PrintPrefix(OutputFile);
	OutputFile << "Postfix: ";
	Tree.PrintPostfix(OutputFile);
	OutputFile << "Infix (with redundant brackets): ";
	Tree.PrintInfix(OutputFile);

	OutputFile.close();
	std::cout << "Done. Check FN2.txt for results." << std::endl;

	return 0;
}
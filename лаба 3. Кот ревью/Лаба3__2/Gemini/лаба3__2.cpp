#include <iostream>
#include <string>
#include <windows.h>

#include "лаба3__2(Header).h"

//FIX_ME: using namespace std загрязняет глобальное пространство имён — запрещено код-стайлом Epic Games
//using namespace std;
// Везде используем явный префикс std::

int main()
{
	SetConsoleCP(1251); // Устанавливаем кодовую страницу для ввода
	SetConsoleOutputCP(1251);

	PrintTaskDescription();

	//FIX_ME: Имена переменных в snake_case не соответствуют PascalCase, принятому в Epic Games
	//string original_word;
	std::string OriginalWord;

	//FIX_ME: Булевые переменные должны иметь префикс 'b' согласно стандарту Epic Games
	//нет аналога в оригинале — переменная добавлена при рефакторинге
	bool bIsInputValid = false;

	while (!bIsInputValid)
	{
		std::cout << "Введите слово: ";
		std::cin >> OriginalWord;

		if (IsValidWord(OriginalWord))
		{
			bIsInputValid = true;
		}
		else
		{
			std::cout << "\nОшибка: допускаются только буквы русского и английского алфавита.\n";
			std::cout << "Пожалуйста, попробуйте ещё раз.\n\n";
		}
	}

	std::cout << "\nНачинаем перемешивание слова \"" << OriginalWord << "\"...\n\n";

	//FIX_ME: Имя переменной в snake_case не соответствует PascalCase Epic Games
	//int attempts = 0;
	int Attempts = ShuffleUntilMatch(OriginalWord);

	std::cout << "\n============================================\n";
	std::cout << "Результат: " << Attempts << " попыток\n";
	std::cout << "============================================\n";

	return 0;
}
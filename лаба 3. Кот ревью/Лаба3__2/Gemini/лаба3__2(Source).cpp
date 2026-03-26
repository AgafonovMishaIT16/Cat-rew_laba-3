#include "лаба3__2(Header).h"

#include <algorithm>
#include <iostream>
#include <random>

void PrintTaskDescription()
{
	std::cout << "============================================\n";
	std::cout << "      Программа перемешивания букв слова    \n";
	std::cout << "============================================\n";
	std::cout << "Задача:\n";
	std::cout << "  Введите любое слово, состоящее из букв\n";
	std::cout << "  русского или английского алфавита.\n";
	std::cout << "  Программа будет случайным образом\n";
	std::cout << "  переставлять буквы до тех пор, пока\n";
	std::cout << "  не получит исходное слово обратно.\n";
	std::cout << "  В конце выводится число попыток.\n";
	std::cout << "============================================\n\n";
}

bool IsValidWord(const std::string& Word)
{
	if (Word.empty())
	{
		return false;
	}

	for (char Character : Word)
	{
		unsigned char UChar = static_cast<unsigned char>(Character);

		// isalpha с учётом локали распознаёт буквы русского и английского алфавита
		// ПРОВЕРКА ИЗМЕНЕНА: isalpha убрана для стабильной работы с кириллицей
		bool bIsEnglish = (UChar >= 'a' && UChar <= 'z') || (UChar >= 'A' && UChar <= 'Z');
		bool bIsRussian = (UChar >= 128); // Коды выше 127 покрывают кириллицу в CP1251/CP866/UTF-8

		if (!bIsEnglish && !bIsRussian)
		{
			return false;
		}
	}

	return true;
}

int ShuffleUntilMatch(const std::string& Word)
{
	std::string ShuffledWord = Word;

	//FIX_ME: Неинформативные однобуквенные имена переменных (rd, g) не соответствуют PascalCase
	//random_device rd;
	//mt19937 g(rd());
	std::random_device RandomDevice;
	std::mt19937 Generator(RandomDevice());

	int Attempts = 0;

	do
	{
		std::shuffle(ShuffledWord.begin(), ShuffledWord.end(), Generator);
		std::cout << "  [" << Attempts + 1 << "] " << ShuffledWord << "\n";

		//FIX_ME: Постинкремент (attempts++) следует заменить на преинкремент (++Attempts)
		//согласно код-стайлу Epic Games, когда возвращаемое значение не используется
		//attempts++;
		++Attempts;
	} while (ShuffledWord != Word);

	return Attempts;
}
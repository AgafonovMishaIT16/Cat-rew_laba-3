#include "лаба3__2GPT(Header).h"

#include <iostream>
#include <algorithm>
#include <random>
#include <vector>

namespace
{
	/** Генератор случайных чисел, локальный для файла. */
	std::random_device RandomDevice;
	std::mt19937 RandomEngine(RandomDevice());
}

namespace WordGame
{
	bool IsValidWord(const std::string& InWord)
	{
		if (InWord.empty())
		{
			return false;
		}

		for (int i = 0; i < (int)InWord.length(); ++i)
		{
			unsigned char Byte = (unsigned char)InWord[i];

			// Разрешаем стандартную латиницу
			if ((Byte >= 'a' && Byte <= 'z') || (Byte >= 'A' && Byte <= 'Z'))
			{
				continue;
			}

			/** * Любой байт > 127 является частью расширенной кодировки (Кириллица).
			 * Это работает и для UTF-8, и для Windows-1251.
			 */
			if (Byte > 127)
			{
				continue;
			}

			// Если символ — цифра, пробел или спецсимвол
			return false;
		}

		return true;
	}

	std::string ShuffleWord(const std::string& InWord)
	{
		/** * Разбиваем строку на блоки байтов.
		 * В UTF-8 русская буква — это 2 байта (начинаются с 0xD0 или 0xD1).
		 */
		std::vector<std::string> Characters;
		for (int i = 0; i < (int)InWord.length(); ++i)
		{
			std::string CurrentChar;
			unsigned char Byte = (unsigned char)InWord[i];

			if (Byte <= 127) // 1-байтовый символ (ASCII)
			{
				CurrentChar = InWord[i];
			}
			else if ((Byte & 0xE0) == 0xC0) // 2-байтовый символ (Кириллица UTF-8)
			{
				CurrentChar = InWord.substr(i, 2);
				i += 1;
			}
			else if ((Byte & 0xF0) == 0xE0) // 3-байтовый символ
			{
				CurrentChar = InWord.substr(i, 3);
				i += 2;
			}
			else // 4-байтовый символ
			{
				CurrentChar = InWord.substr(i, 4);
				i += 3;
			}
			Characters.push_back(CurrentChar);
		}

		std::shuffle(Characters.begin(), Characters.end(), RandomEngine);

		std::string ShuffledResult;
		for (const std::string& CharStr : Characters)
		{
			ShuffledResult += CharStr;
		}

		return ShuffledResult;
	}

	void RunWordShuffle()
	{
		std::string OriginalWord;

		/** * Внешний цикл: работаем до тех пор, пока не введено корректное слово.
		 */
		while (true)
		{
			std::cout << "Введите слово (русские или английские буквы): ";
			std::cin >> OriginalWord;

			if (IsValidWord(OriginalWord))
			{
				// Если слово валидно, выходим из цикла ввода
				break;
			}

			std::cout << "Ошибка: в слове найдены недопустимые символы. Попробуйте снова.\n";
		}

		std::string CurrentWord;
		int AttemptCount = 0;

		/** Цикл перемешивания до совпадения с оригиналом. */
		do
		{
			CurrentWord = ShuffleWord(OriginalWord);
			std::cout << CurrentWord << std::endl;

			AttemptCount++;

		} while (CurrentWord != OriginalWord);

		std::cout << "Успех! Потребовалось " << AttemptCount - 1 << " попыток перемешивания." << std::endl;
	}
}
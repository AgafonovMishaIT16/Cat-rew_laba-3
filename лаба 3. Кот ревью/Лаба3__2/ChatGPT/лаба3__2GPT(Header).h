#pragma once

#include <string>

/**
 * Пространство имен для логики игры в перемешивание слов.
 */
namespace WordGame
{
	/**
	 * Проверяет, содержит ли строка только буквы (латиница + символы для кириллицы).
	 */
	bool IsValidWord(const std::string& InWord);

	/**
	 * Перемешивает символы в строке.
	 */
	std::string ShuffleWord(const std::string& InWord);

	/**
	 * Запускает бесконечный цикл ввода и перемешивания слова.
	 */
	void RunWordShuffle();
}
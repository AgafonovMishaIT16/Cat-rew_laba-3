#pragma once

#include <string>

/**
 * Выводит описание задачи и приветственное сообщение в консоль.
 */
void PrintTaskDescription();

/**
 * Проверяет, состоит ли строка только из букв русского и английского алфавита.
 * @param Word — проверяемое слово
 * @return true, если слово валидно; false в противном случае
 */
bool IsValidWord(const std::string& Word);

/**
 * Перемешивает буквы слова случайным образом до тех пор,
 * пока результат не совпадёт с исходным словом.
 * @param Word — исходное слово
 * @return количество потребовавшихся попыток
 */
int ShuffleUntilMatch(const std::string& Word);
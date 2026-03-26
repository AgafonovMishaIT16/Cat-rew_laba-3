#include "лаба3__CL2.Header.h"
#include <algorithm>
#include <iostream>
#include <random>

namespace
{
    /**
     * Возвращает true, если Ch является латинской буквой верхнего
     * или нижнего регистра (диапазон ASCII).
     */
    bool IsLatinLetter(wchar_t Ch)
    {
        return (Ch >= L'a' && Ch <= L'z')
            || (Ch >= L'A' && Ch <= L'Z');
    }

    /**
     * Возвращает true, если Ch является буквой кириллического алфавита,
     * включая «ё» / «Ё», которые находятся за пределами основного
     * непрерывного блока Unicode.
     */
    bool IsCyrillicLetter(wchar_t Ch)
    {

        const bool bMainBlock = (Ch >= L'\u0410' && Ch <= L'\u044F');

        const bool bYo = (Ch == L'\u0401' || Ch == L'\u0451');

        return bMainBlock || bYo;
    }

}


bool IsValidWord(const std::wstring& Word)
{
    if (Word.empty())
    {
        std::wcerr << L"Ошибка: введена пустая строка.\n";
        return false;
    }

    if (static_cast<int>(Word.size()) > GMaxWordLength)
    {
        std::wcerr << L"Ошибка: слово слишком длинное (максимум "
            << GMaxWordLength << L" букв).\n";
        return false;
    }

    for (const wchar_t Ch : Word)
    {
        if (!IsLatinLetter(Ch) && !IsCyrillicLetter(Ch))
        {
            std::wcerr << L"Ошибка: недопустимый символ '"
                << Ch
                << L"'. Разрешены только русские и английские буквы.\n";
            return false;
        }
    }

    return true;
}


int ShuffleUntilMatch(const std::wstring& OriginalWord)
{

    std::random_device TrueRandom;
    std::mt19937       RandomEngine(TrueRandom());

    std::wstring ShuffledWord = OriginalWord;
    int          AttemptCount = 0;

    do
    {
        std::shuffle(ShuffledWord.begin(), ShuffledWord.end(), RandomEngine);
        ++AttemptCount;

        /* Выводим текущую перестановку и её порядковый номер. */
        std::wcout << ShuffledWord << L"  [" << AttemptCount << L"]\n";
    } while (ShuffledWord != OriginalWord);

    return AttemptCount;
}
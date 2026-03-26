// -----------------------------------------------------------------------
//  Main.cpp
//  Точка входа: настройка локали, ввод пользователя, цикл валидации,
//  вывод результата.
//  Стандарт кодирования: Epic Games / Unreal Engine Style Guide
// -----------------------------------------------------------------------
#include "лаба3__CL2.Header.h"
#include <iostream>
#include <locale>
#include <string>

#ifdef _WIN32
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#endif

namespace
{
    /**
     * Переводит стандартные потоки ввода/вывода в режим UTF-16 (Windows)
     * или настраивает системную локаль (Linux/macOS), чтобы широкосимвольный
     * ввод-вывод корректно работал как для кириллицы, так и для латиницы.
     *
     * Примечание: на Windows используется _setmode с флагом _O_U16TEXT —
     * более надёжный способ, чем SetConsoleCP(65001) + imbue,
     * который ломает std::wcin при вводе кириллицы.
     */
    void SetupLocale()
    {
#ifdef _WIN32

        _setmode(_fileno(stdin), _O_U16TEXT);
        _setmode(_fileno(stdout), _O_U16TEXT);
        _setmode(_fileno(stderr), _O_U16TEXT);
#else

        std::locale SystemLocale("");
        std::locale::global(SystemLocale);
        std::wcin.imbue(SystemLocale);
        std::wcout.imbue(SystemLocale);
#endif
    }

}


int main()
{
    SetupLocale();


    std::wstring InputWord;
    while (true)
    {
        std::wcout << L"Введите слово (только рус./англ. буквы, не более "
            << GMaxWordLength << L" символов): ";

        if (!(std::wcin >> InputWord))
        {
            std::wcerr << L"Ошибка чтения ввода.\n";
            return 1;
        }

        if (IsValidWord(InputWord))
        {
            break;
        }

        /* IsValidWord уже вывела конкретную ошибку; просим ввести снова. */
        std::wcout << L"Пожалуйста, попробуйте ещё раз.\n\n";
    }


    /*  Перемешиваем слово, пока оно не вернётся к исходному виду.*/
    std::wcout << L'\n';
    const int TotalAttempts = ShuffleUntilMatch(InputWord);


    /*  Итоговый вывод с правильным склонением слова «попытка» . */

    std::wcout << L'\n'
        << TotalAttempts
        << L" попыт"
        << (TotalAttempts % 10 == 1 && TotalAttempts % 100 != 11
            ? L"ка"
            : (TotalAttempts % 10 >= 2 && TotalAttempts % 10 <= 4
                && (TotalAttempts % 100 < 10 || TotalAttempts % 100 >= 20)
                ? L"ки"
                : L"ок"))
        << L'\n';

    return 0;
}
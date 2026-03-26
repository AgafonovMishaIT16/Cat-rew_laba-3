#include "Header.h"
#include <windows.h>

#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>


/** Считывает все содержимое FilePath в OutContent.
 * return true в случае успеха. */
static bool ReadFileToString(const std::string& FilePath, std::string& OutContent)
{
    std::ifstream File(FilePath);
    if (!File.is_open())
    {
        return false;
    }

    std::ostringstream Buffer;
    Buffer << File.rdbuf();
    OutContent = Buffer.str();
    return true;
}

/** Удаляет пробельные символы и символы переноса строки с конца строки на месте. */
static void TrimTrailing(std::string& Str)
{
    while (!Str.empty() && std::isspace(static_cast<unsigned char>(Str.back())))
    {
        Str.pop_back();
    }
}

/**
 * Считывает одно значение int из стандартного ввода с проверкой.
 * Запрашивает ввод в цикле, пока пользователь не предоставит корректное целое число.
 */
static int ReadIntFromUser(const std::string& Prompt)
{
    std::string InputStr;
    while (true)
    {
        std::cout << Prompt;

        // Считываем строку целиком
        if (std::getline(std::cin, InputStr))
        {
            // Пропускаем случайный ввод пустой строки (например, случайное нажатие Enter)
            if (InputStr.empty())
            {
                continue;
            }

            try
            {
                size_t Pos = 0;
                const int Value = std::stoi(InputStr, &Pos);

                // Проверяем, не осталось ли после числа символов, отличных от пробелов
                bool bIsValid = true;
                for (size_t i = Pos; i < InputStr.length(); ++i)
                {
                    if (!std::isspace(static_cast<unsigned char>(InputStr[i])))
                    {
                        bIsValid = false;
                        break;
                    }
                }

                // Если строка состоит строго из числа — возвращаем его
                if (bIsValid)
                {
                    return Value;
                }
            }
            catch (const std::exception&)
            {
                // std::stoi выбросит исключение при невозможности парсинга
                // или выходе за пределы допустимого диапазона значений
            }
        }
        else
        {
            // Очищаем состояние потока на случай EOF или критической ошибки
            std::cin.clear();
        }

        std::cerr << "  [!] Неверный ввод — пожалуйста, введите целое число.\n";
    }
}


int main()
{

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    const std::string InputFileName = "FN1.txt";
    const std::string OutputFileName = "FN2.txt";

    // 1. Чтение инфиксного выражения из FN1.txt 
    std::string RawExpression;
    if (!ReadFileToString(InputFileName, RawExpression))
    {
        std::cerr << "[Ошибка] Не удалось открыть входной файл '" << InputFileName << "'.\n";
        return 1;
    }

    TrimTrailing(RawExpression);

    if (RawExpression.empty())
    {
        std::cerr << "[Ошибка] Входной файл '" << InputFileName << "' пуст.\n";
        return 1;
    }

    // ── 2. Валидация исходных символов
    std::string ValidationError;
    if (!ValidateRawInput(RawExpression, ValidationError))
    {
        std::cerr << "[Ошибка валидации] " << ValidationError << '\n';
        return 1;
    }

    // 3. Вывод выражения и запрос значения x
    std::cout << "Выражение, прочитанное из '" << InputFileName << "':\n";
    std::cout << "  " << RawExpression << "\n\n";

    const int XValue = ReadIntFromUser("Введите значение x: ");
    std::cout << '\n';

    // 4. Построение дерева выражений
    FExpressionTree Tree;
    if (!Tree.Build(RawExpression))
    {
        std::cerr << "[Ошибка парсинга] " << Tree.GetLastError() << '\n';
        return 1;
    }

    // 5. Вычисление дерева (до любых преобразований)
    int Result = 0;
    try
    {
        Result = Tree.Evaluate(XValue);
    }
    catch (const std::runtime_error& Ex)
    {
        std::cerr << "[Ошибка вычисления] " << Ex.what() << '\n';
        return 1;
    }

    // 6. Открытие выходного файла
    std::ofstream OutputFile(OutputFileName);
    if (!OutputFile.is_open())
    {
        std::cerr << "[Ошибка] Не удалось открыть выходной файл '" << OutputFileName << "'.\n";
        return 1;
    }

    // 7. Запись результата вычисления
    OutputFile << "=== Исходное выражение ===\n";
    OutputFile << "Выражение  : " << RawExpression << '\n';
    OutputFile << "x          = " << XValue << '\n';
    OutputFile << "Результат  = " << Result << '\n';
    OutputFile << '\n';

    //  8. Применение преобразования  A + x  →  x + A
    Tree.TransformAddX();

    //  9. Запись трех форм преобразованного дерева
    OutputFile << "=== После преобразования  (A + x  ->  x + A) ===\n";
    OutputFile << "Префикс  : " << Tree.ToPrefixString() << '\n';
    OutputFile << "Постфикс : " << Tree.ToPostfixString() << '\n';
    OutputFile << "Инфикс   : " << Tree.ToInfixString() << '\n';

    OutputFile.close();

    // 10. Вывод итога в stdout
    std::cout << "Результат для x = " << XValue << "  =>  " << Result << '\n';
    std::cout << "Все результаты записаны в '" << OutputFileName << "'.\n";

    return 0;
}
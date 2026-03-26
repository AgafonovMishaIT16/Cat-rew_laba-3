#include "Header.h"
#include <fstream>
#include <iostream>

bool ReadGraph(const std::string& InFilename, int& OutVertexCount, std::vector<std::vector<int>>& OutAdjMatrix)
{
    std::ifstream InFile(InFilename);
    if (!InFile.is_open())
    {
        std::cerr << "Ошибка: не удалось открыть файл " << InFilename << std::endl;
        return false;
    }

    if (!(InFile >> OutVertexCount))
    {
        std::cerr << "Ошибка: не удалось прочитать количество вершин." << std::endl;
        return false;
    }

    if (OutVertexCount <= 0)
    {
        std::cerr << "Ошибка: количество вершин должно быть положительным, получено " << OutVertexCount << std::endl;
        return false;
    }

    OutAdjMatrix.assign(OutVertexCount, std::vector<int>(OutVertexCount, 0));

    for (int I = 0; I < OutVertexCount; ++I)
    {
        for (int J = 0; J < OutVertexCount; ++J)
        {
            if (!(InFile >> OutAdjMatrix[I][J]))
            {
                std::cerr << "Ошибка: недостаточно данных в файле." << std::endl;
                return false;
            }

            if (OutAdjMatrix[I][J] != 0 && OutAdjMatrix[I][J] != 1)
            {
                std::cerr << "Ошибка: элемент матрицы смежности должен быть 0 или 1, но найден " << OutAdjMatrix[I][J] << std::endl;
                return false;
            }
        }
    }

    // Проверка симметричности и отсутствия петель
    for (int I = 0; I < OutVertexCount; ++I)
    {
        if (OutAdjMatrix[I][I] != 0)
        {
            std::cerr << "Ошибка: обнаружена петля на вершине " << I + 1 << std::endl;
            return false;
        }

        for (int J = I + 1; J < OutVertexCount; ++J)
        {
            if (OutAdjMatrix[I][J] != OutAdjMatrix[J][I])
            {
                std::cerr << "Ошибка: матрица несимметрична для пары (" << I + 1 << "," << J + 1 << ")" << std::endl;
                return false;
            }
        }
    }

    InFile.close();
    return true;
}

bool BuildIncidenceMatrix(int InVertexCount, const std::vector<std::vector<int>>& InAdjMatrix,
    std::vector<std::vector<int>>& OutIncMatrix, int& OutEdgeCount)
{
    OutEdgeCount = 0;
    for (int I = 0; I < InVertexCount; ++I)
    {
        for (int J = I + 1; J < InVertexCount; ++J)
        {
            if (InAdjMatrix[I][J] == 1)
            {
                ++OutEdgeCount;
            }
        }
    }

    OutIncMatrix.assign(InVertexCount, std::vector<int>(OutEdgeCount, 0));

    int CurrentColumn = 0;
    for (int I = 0; I < InVertexCount; ++I)
    {
        for (int J = I + 1; J < InVertexCount; ++J)
        {
            if (InAdjMatrix[I][J] == 1)
            {
                OutIncMatrix[I][CurrentColumn] = 1;
                OutIncMatrix[J][CurrentColumn] = 1;
                ++CurrentColumn;
            }
        }
    }

    return true;
}

bool WriteIncidenceMatrix(const std::string& InFilename, int InVertexCount, int InEdgeCount,
    const std::vector<std::vector<int>>& InIncMatrix)
{
    std::ofstream OutFile(InFilename);
    if (!OutFile.is_open())
    {
        std::cerr << "Ошибка: не удалось открыть файл для записи " << InFilename << std::endl;
        return false;
    }

    OutFile << InVertexCount << " " << InEdgeCount << "\n";

    for (int I = 0; I < InVertexCount; ++I)
    {
        for (int J = 0; J < InEdgeCount; ++J)
        {
            OutFile << InIncMatrix[I][J];
            if (J != InEdgeCount - 1)
            {
                OutFile << " ";
            }
        }

        if (I != InVertexCount - 1)
        {
            OutFile << "\n";
        }
    }

    OutFile.close();
    return true;
}
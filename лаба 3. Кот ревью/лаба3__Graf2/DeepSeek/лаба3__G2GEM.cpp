#include "Header.h"
#include <iostream>

int main(int ArgC, char* ArgV[])
{
    setlocale(LC_ALL, "Russian");

    std::string InputFileName = "Fn1.txt";
    std::string OutputFileName = "Fn2.txt";

    if (ArgC >= 3)
    {
        InputFileName = ArgV[1];
        OutputFileName = ArgV[2];
    }
    else if (ArgC == 2)
    {
        InputFileName = ArgV[1];
    }

    std::cout << "Чтение графа из файла: " << InputFileName << std::endl;

    int VertexCount = 0;
    std::vector<std::vector<int>> AdjacencyMatrix;

    const bool bReadSuccess = ReadGraph(InputFileName, VertexCount, AdjacencyMatrix);
    if (!bReadSuccess)
    {
        std::cerr << "Ошибка при чтении графа. Программа завершена." << std::endl;
        return 1;
    }

    std::cout << "Граф успешно прочитан. Количество вершин: " << VertexCount << std::endl;

    int EdgeCount = 0;
    std::vector<std::vector<int>> IncidenceMatrix;

    BuildIncidenceMatrix(VertexCount, AdjacencyMatrix, IncidenceMatrix, EdgeCount);
    std::cout << "Матрица инцидентности построена. Количество ребер: " << EdgeCount << std::endl;

    const bool bWriteSuccess = WriteIncidenceMatrix(OutputFileName, VertexCount, EdgeCount, IncidenceMatrix);
    if (!bWriteSuccess)
    {
        std::cerr << "Ошибка при записи матрицы в файл." << std::endl;
        return 1;
    }

    std::cout << "Матрица инцидентности успешно записана в файл: " << OutputFileName << std::endl;

    return 0;
}
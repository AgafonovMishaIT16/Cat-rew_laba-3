#pragma once

#include <string>
#include <vector>

/**
 * Читает граф из файла и заполняет матрицу смежности.
 *
 * @param InFilename      Путь к исходному файлу.
 * @param OutVertexCount  Переменная для записи количества вершин.
 * @param OutAdjMatrix    Матрица для записи связей.
 * @return True, если операция прошла успешно.
 */
bool ReadGraph(const std::string& InFilename, int& OutVertexCount, std::vector<std::vector<int>>& OutAdjMatrix);

/**
 * Построение матрицы инцидентности на основе матрицы смежности.
 */
bool BuildIncidenceMatrix(int InVertexCount, const std::vector<std::vector<int>>& InAdjMatrix,
    std::vector<std::vector<int>>& OutIncMatrix, int& OutEdgeCount);

/**
 * Запись полученной матрицы в текстовый файл.
 */
bool WriteIncidenceMatrix(const std::string& InFilename, int InVertexCount, int InEdgeCount,
    const std::vector<std::vector<int>>& InIncMatrix);
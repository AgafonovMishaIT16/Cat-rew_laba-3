#pragma once

#include <vector>
#include <string>

/** Структура для хранения данных графа */
struct FGraphData
{
	int NumVertices = 0;
	int NumEdges = 0;
	std::vector<std::vector<int>> AdjacencyMatrix;
};

/** Структура ребра для формирования матрицы инцидентности */
struct FEdge
{
	int FirstVertex;
	int SecondVertex;
};

namespace GraphConverter
{
	/** Считывает матрицу из файла с валидацией */
	bool LoadAdjacencyMatrix(const std::string& InFileName, FGraphData& OutGraph);

	/** Генерирует и записывает матрицу инцидентности в файл */
	bool SaveIncidenceMatrix(const std::string& InFileName, const FGraphData& Graph);
}
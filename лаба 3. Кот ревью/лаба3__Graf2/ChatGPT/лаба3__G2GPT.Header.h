#pragma once

#include <vector>
#include <string>

/**
 * Класс для работы с графом.
 * Используется префикс F (согласно Epic Coding Standard для обычных классов/структур).
 */
class FGraph
{
public:
	FGraph();

	/** Чтение графа из файла. Возвращает true в случае успеха. */
	bool ReadAdjacencyMatrix(const std::string& InFileName);

	/** Построение матрицы инцидентности на основе матрицы смежности. */
	void BuildIncidenceMatrix();

	/** Запись матрицы инцидентности в файл. */
	bool WriteIncidenceMatrix(const std::string& InFileName) const;

private:
	/** Матрица смежности. */
	std::vector<std::vector<int>> AdjacencyMatrix;

	/** Матрица инцидентности. */
	std::vector<std::vector<int>> IncidenceMatrix;

	/** Количество вершин в графе. */
	int NumVertices;

	/** Количество рёбер в графе. */
	int NumEdges;

	/** Валидация считанной матрицы смежности. */
	bool ValidateAdjacencyMatrix() const;
};
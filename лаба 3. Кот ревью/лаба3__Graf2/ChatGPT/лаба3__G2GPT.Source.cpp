#include "лаба3__G2GPT.Header.h"
#include <fstream>
#include <iostream>
#include <algorithm>

FGraph::FGraph()
	: NumVertices(0)
	, NumEdges(0)
{
}

bool FGraph::ReadAdjacencyMatrix(const std::string& InFileName)
{
	std::ifstream InFile(InFileName);
	if (!InFile.is_open())
	{
		return false;
	}

	InFile >> NumVertices;
	if (NumVertices <= 0)
	{
		return false;
	}

	// Инициализация матрицы
	AdjacencyMatrix.assign(NumVertices, std::vector<int>(NumVertices, 0));

	for (int i = 0; i < NumVertices; ++i)
	{
		for (int j = 0; j < NumVertices; ++j)
		{
			if (!(InFile >> AdjacencyMatrix[i][j]))
			{
				return false;
			}
		}
	}

	return ValidateAdjacencyMatrix();
}

bool FGraph::ValidateAdjacencyMatrix() const
{
	for (int i = 0; i < NumVertices; ++i)
	{
		
		if (AdjacencyMatrix[i][i] != 0)
		{
			return false;
		}

		for (int j = 0; j < NumVertices; ++j)
		{
			/* Проверка симметричности и допустимых значений(0 или 1) */
			const int Value = AdjacencyMatrix[i][j];
			if (Value != AdjacencyMatrix[j][i] || (Value != 0 && Value != 1))
			{
				return false;
			}
		}
	}
	return true;
}

void FGraph::BuildIncidenceMatrix()
{
	/* Временная структура для хранения ребер(имена структур тоже с F) */
	struct FEdge
	{
		int U;
		int V;
	};

	std::vector<FEdge> Edges;

	for (int i = 0; i < NumVertices; ++i)
	{
		for (int j = i + 1; j < NumVertices; ++j)
		{
			if (AdjacencyMatrix[i][j] == 1)
			{
				Edges.push_back({ i, j });
			}
		}
	}

	NumEdges = static_cast<int>(Edges.size());
	IncidenceMatrix.assign(NumVertices, std::vector<int>(NumEdges, 0));

	for (int k = 0; k < NumEdges; ++k)
	{
		const int VertexU = Edges[k].U;
		const int VertexV = Edges[k].V;

		IncidenceMatrix[VertexU][k] = 1;
		IncidenceMatrix[VertexV][k] = 1;
	}
}

bool FGraph::WriteIncidenceMatrix(const std::string& InFileName) const
{
	std::ofstream OutFile(InFileName);
	if (!OutFile.is_open())
	{
		return false;
	}

	OutFile << NumVertices << " " << NumEdges << "\n";
	for (const std::vector<int>& Row : IncidenceMatrix)
	{
		for (size_t j = 0; j < Row.size(); ++j)
		{
			OutFile << Row[j] << (j + 1 == Row.size() ? "" : " ");
		}
		OutFile << "\n";
	}

	return true;
}
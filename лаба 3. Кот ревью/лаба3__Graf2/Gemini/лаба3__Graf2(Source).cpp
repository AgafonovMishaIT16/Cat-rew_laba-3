#include "лаба3__Graf2(Header).h"
#include <fstream>
#include <iostream>

namespace GraphConverter
{
	bool LoadAdjacencyMatrix(const std::string& InFileName, FGraphData& OutGraph)
	{
		std::ifstream InFile(InFileName);
		if (!InFile.is_open())
		{
			std::cerr << "Error: Could not open file " << InFileName << std::endl;
			return false;
		}

		if (!(InFile >> OutGraph.NumVertices) || OutGraph.NumVertices <= 0)
		{
			std::cerr << "Error: Invalid number of vertices." << std::endl;
			return false;
		}

		OutGraph.AdjacencyMatrix.resize(OutGraph.NumVertices, std::vector<int>(OutGraph.NumVertices));

		for (int i = 0; i < OutGraph.NumVertices; ++i)
		{
			for (int j = 0; j < OutGraph.NumVertices; ++j)
			{
				if (!(InFile >> OutGraph.AdjacencyMatrix[i][j]))
				{
					std::cerr << "Error: Matrix data missing or invalid format." << std::endl;
					return false;
				}

				// ¬алидаци€: значени€ должны быть 0 или 1
				if (OutGraph.AdjacencyMatrix[i][j] != 0 && OutGraph.AdjacencyMatrix[i][j] != 1)
				{
					std::cerr << "Error: Adjacency matrix must contain only 0 or 1." << std::endl;
					return false;
				}
			}
		}

		// ¬алидаци€ на симметричность (неориентированный граф)
		for (int i = 0; i < OutGraph.NumVertices; ++i)
		{
			for (int j = 0; j < OutGraph.NumVertices; ++j)
			{
				if (OutGraph.AdjacencyMatrix[i][j] != OutGraph.AdjacencyMatrix[j][i])
				{
					std::cerr << "Error: Matrix is not symmetric (not an undirected graph)." << std::endl;
					return false;
				}
				// –ебра на петли (i == j) обычно игнорируютс€ или обрабатываютс€ отдельно, 
				// здесь проверим отсутствие петель дл€ простоты.
				if (i == j && OutGraph.AdjacencyMatrix[i][j] != 0)
				{
					std::cerr << "Warning: Self-loops detected." << std::endl;
				}
			}
		}

		return true;
	}

	bool SaveIncidenceMatrix(const std::string& InFileName, const FGraphData& Graph)
	{
		std::vector<FEdge> Edges;

		// —бор ребер согласно правилу: сначала от 1-й вершины, потом от 2-й и т.д.
		// „тобы не дублировать ребра (i,j) и (j,i), берем только i < j.
		for (int i = 0; i < Graph.NumVertices; ++i)
		{
			for (int j = i + 1; j < Graph.NumVertices; ++j)
			{
				if (Graph.AdjacencyMatrix[i][j] == 1)
				{
					Edges.push_back({ i, j });
				}
			}
		}

		const int NumEdges = static_cast<int>(Edges.size());
		std::ofstream OutFile(InFileName);
		if (!OutFile.is_open())
		{
			return false;
		}

		// ¬ывод размеров n и m
		OutFile << Graph.NumVertices << " " << NumEdges << "\n";

		// ѕостроение и вывод строк матрицы инцидентности
		for (int i = 0; i < Graph.NumVertices; ++i)
		{
			for (int j = 0; j < NumEdges; ++j)
			{
				bool bIsIncident = (Edges[j].FirstVertex == i || Edges[j].SecondVertex == i);
				OutFile << (bIsIncident ? 1 : 0) << (j == NumEdges - 1 ? "" : " ");
			}
			OutFile << "\n";
		}

		return true;
	}
}
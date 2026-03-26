#include "лаба3__Graf2(Header).h"
#include <iostream>

int main()
{
	const std::string InputFileName = "FileName1.txt";
	const std::string OutputFileName = "FF.txt";

	FGraphData Graph;

	// Загрузка и валидация
	if (!GraphConverter::LoadAdjacencyMatrix(InputFileName, Graph))
	{
		std::cerr << "Failed to process input file." << std::endl;
		return 1;
	}

	// Обработка и сохранение
	if (!GraphConverter::SaveIncidenceMatrix(OutputFileName, Graph))
	{
		std::cerr << "Failed to write to output file." << std::endl;
		return 1;
	}

	std::cout << "Success! Incidence matrix saved to " << OutputFileName << std::endl;

	return 0;
}
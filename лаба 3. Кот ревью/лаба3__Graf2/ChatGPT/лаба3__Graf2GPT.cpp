#include "лаба3__G2GPT.Header.h"
#include <iostream>
#include <string>

int main()
{
	const std::string InputFileName = "GPT1.txt";
	const std::string OutputFileName = "GPT2.txt";

	FGraph GraphInstance;

	if (!GraphInstance.ReadAdjacencyMatrix(InputFileName))
	{
		std::cerr << "Error: Could not read adjacency matrix." << std::endl;
		return 1;
	}

	GraphInstance.BuildIncidenceMatrix();

	if (!GraphInstance.WriteIncidenceMatrix(OutputFileName))
	{
		std::cerr << "Error: Could not write incidence matrix." << std::endl;
		return 1;
	}

	std::cout << "Success: Incidence matrix saved to " << OutputFileName << std::endl;
	return 0;
}
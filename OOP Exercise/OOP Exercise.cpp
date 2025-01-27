#include "InfoOutputter.h"
#include "InputStreamInterface.h"
#include "TradeManager.h"

#include <iostream>
#include <chrono>

int main()
{
	std::unique_ptr<InputInterface> interface { std::make_unique<CSVInputInterface>() };
	HashTableTradeManager manager(std::move(interface));
	CSVInfoOutputter outputter;

	std::ifstream file;
	file.open("input.csv");
	std::string filename = "output.csv";
	
	
	if (file.is_open()) {
		auto start = std::chrono::high_resolution_clock::now();
		manager.generateDataFromStream(file);
		auto sorted_data = manager.dumpData();
		outputter.writeToFile(filename, sorted_data);

		auto stop = std::chrono::high_resolution_clock::now();
		auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		std::cout << "Successfully wrote to " << filename << '.' << std::endl;
		std::cout << "Operation took: " << time << "." << std::endl;
	}
	return 0;
}


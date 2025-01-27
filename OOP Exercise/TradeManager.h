#pragma once

#include "OOP Exercise.h"
#include "InputStreamInterface.h"
#include <vector>
#include <unordered_map>
#include <algorithm>

// Interface for the main class of my solution.  Initalized only with an input manager.  
// Outputs a sorted vector of symbolData structs.
class TradeManager
{
public:
	TradeManager() = delete;
	TradeManager(std::unique_ptr<InputInterface>&& interface) : lineParser(std::move(interface)){};
	virtual void generateDataFromStream(std::istream& input_stream) = 0;
	virtual std::vector<outputEntry> dumpData() const = 0;

protected:
	std::unique_ptr<InputInterface> lineParser;
};

// Implementation of the abstract class that uses an unordered map to organize symbol data.
// Alternatives for potential performance improvements include: using a 3D array for 26*26*26 3 character symbols (saves on hashing)
// or a standard map, which saves on sorting when outputting the output.csv file, at cost of nonconstant insert.

class HashTableTradeManager : public TradeManager
{
public:
	HashTableTradeManager(std::unique_ptr<InputInterface>&& interface) : TradeManager(std::move(interface)) {};
	void generateDataFromStream(std::istream& input_stream) override;
	std::vector<outputEntry> dumpData() const override;

private:
	void addToMap(inputData& data);
	void update_entry(inputData& data, outputEntry& entry);
	std::unordered_map<std::string, outputEntry> table;
};


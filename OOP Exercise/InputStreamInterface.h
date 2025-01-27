#pragma once

#include "OOP Exercise.h"
#include <istream>
#include <sstream>

// Manages pulling converting a line from the input stream to a set of values.

class InputInterface
{
public:
	virtual std::optional<inputData> parseInfoFromString(const std::string& line) const = 0;
};

// Interprets a line from the input stream in CSV format.
// I wanted to have it read based on number of tuple args, but 
// that requires template metaprogramming which I'm not quite comfortable with.
// For now it's hardcoded to 4.
class CSVInputInterface : public InputInterface 
{
public:
	std::optional<inputData> parseInfoFromString(const std::string& line) const override;
private:
	const char delim = ',';
};
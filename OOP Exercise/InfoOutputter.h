#pragma once

#include "OOP Exercise.h"
#include <vector>
#include <sstream>
#include <fstream>

// Takes the output vector of symbolData, and outputs it to a text file.  In this case, CSV.
class InfoOutputter
{
public:
	// True or false indicates success of write
	virtual bool writeToFile(const std::string& filename, const std::vector<outputEntry>& output_vector) const = 0;
	// Write to string primarily for testing purposes.
	virtual std::string writeToString(const std::vector<outputEntry>& output_vector) const = 0;
};

class CSVInfoOutputter : public InfoOutputter 
{
public:
	bool writeToFile(const std::string& filename, const std::vector<outputEntry>& output_vector) const override;
	std::string writeToString(const std::vector<outputEntry>& output_vector) const override;

private:
	const char delim = ',';

	void writeVecToStream(const std::vector<outputEntry>& vec, std::ostream& stream) const;
};

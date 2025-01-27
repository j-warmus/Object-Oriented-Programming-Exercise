#include "InfoOutputter.h"

bool CSVInfoOutputter::writeToFile(const std::string& filename, const std::vector<outputEntry>& output_vector) const
{
	std::ofstream file;
	file.open(filename, std::ios::trunc | std::ios::out);
	if (!file.is_open()) {
		return false;
	}

	writeVecToStream(output_vector, file);
	file.close();
	return true;
}

std::string CSVInfoOutputter::writeToString(const std::vector<outputEntry>& output_vector) const 
{
	std::ostringstream out;
	writeVecToStream(output_vector, out);
	return out.str();
}

void CSVInfoOutputter::writeVecToStream(const std::vector<outputEntry>& vec, std::ostream& stream) const
{
	for (auto it = vec.begin(); it != vec.end(); ++it) {
		stream << it->symbol << delim;
		stream << it->maxTimeGap << delim;
		stream << it->volume << delim;
		stream << (long int)it->weightedAvgPrice << delim;
		stream << it->maxPrice << '\n';
	}
	return;
}

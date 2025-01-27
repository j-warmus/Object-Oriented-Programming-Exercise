#include "TradeManager.h"

void HashTableTradeManager::generateDataFromStream(std::istream& input_stream)
{
	std::string s;
	
	// Parses until no more lines.  Skips malformed lines (according to lineParser)
	while (getline(input_stream, s)) {
		auto vals = lineParser->parseInfoFromString(s);
		if (vals.has_value()) {
			addToMap(vals.value());
		}
	}
	return;
}

// This is quite slow, a lot of copying.
// Assuming infrequent use case, not so big of a deal
std::vector<outputEntry> HashTableTradeManager::dumpData() const
{
	std::vector<outputEntry> out;
	out.reserve(table.size());

	for (const auto& [key, value] : table) {
		out.push_back(value);
	}

	auto compareEntries = [](outputEntry first, outputEntry second) {return first.symbol < second.symbol; };
	std::sort(out.begin(), out.end(), compareEntries);
	
	return out;
}

void HashTableTradeManager::addToMap(inputData& data)
{
	auto [timestamp, symbol, quantity, price] = data;
	
	if (const auto it = table.find(symbol); it != table.end()) {
		outputEntry& entry = it->second;
		update_entry(data, entry);
	}
	else {
		// create
		table[symbol] = outputEntry{ symbol, 0, timestamp, quantity, static_cast<double>(price), price };
	}
}

void HashTableTradeManager::update_entry(inputData& data, outputEntry& entry)
{
	auto& [timestamp, symbol, quantity, price] = data;

	entry.maxTimeGap = std::max(timestamp - entry.lastTime, entry.maxTimeGap);
	entry.lastTime = timestamp;
	double total = entry.volume + (double)quantity;
	entry.weightedAvgPrice = entry.weightedAvgPrice * entry.volume / total + (double)price * quantity / total;
	entry.volume += quantity;
	entry.maxPrice = std::max(price, entry.maxPrice);
	
}

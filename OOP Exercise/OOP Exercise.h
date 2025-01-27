#pragma once

#include <tuple>
#include <string>
#include <optional>

using microseconds = unsigned long long int;
// <TimeStamp>, <Symbol>, <Quantity>, <Price>
using inputData = std::tuple <microseconds, std::string, int, unsigned int>;

// struct for holding info on each ticker symbol. 
// Note weightedAvgPrice is a double, even though the final result is an integer. 

struct outputEntry {
	std::string symbol;
	microseconds maxTimeGap;
	microseconds lastTime;
	int volume;
	double weightedAvgPrice;
	unsigned int maxPrice;
};
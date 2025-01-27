#include "InputStreamInterface.h"

// Gets four values.  If any of them fail, return empty optional. Extra (beyond 4) are discarded.

std::optional<inputData> CSVInputInterface::parseInfoFromString(const std::string& line) const
{
	std::string a, b, c, d;
	std::stringstream ss(line);
	inputData out;

	if (getline(ss, a, ',') && getline(ss, b, ',') && getline(ss, c, ',') && getline(ss, d))
	{
		std::stringstream sa(a), sb(b), sc(c), sd(d);
		sa >> std::get<0>(out);
		sb >> std::get<1>(out);
		sc >> std::get<2>(out);
		sd >> std::get<3>(out);

		if (sa.fail() || sb.fail() || sc.fail() || sd.fail() || ss.fail() || std::get<1>(out).size() != 3) {
			return std::optional<inputData>{};
		}

		return std::optional(out);
	}else{ 
		return std::optional<inputData>{}; 
	}
	
}

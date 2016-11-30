#include "IngameTime.h"
#include <string>

std::string IngameDateToString(IngameDate id)
{
	auto t = std::chrono::system_clock::to_time_t(id);
	std::string s;
	s.resize(100, '\0');
	ctime_s(&s[0], s.size(), &t);
	return s;
}

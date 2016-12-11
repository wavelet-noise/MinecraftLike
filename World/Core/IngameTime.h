#include <chrono>
#include <string>

using IngameDate = std::chrono::time_point<std::chrono::system_clock>;

std::string IngameDateToString(IngameDate id);
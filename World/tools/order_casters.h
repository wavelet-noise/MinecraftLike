#pragma once
#ifndef ORDER_CASTERS_H_
#define ORDER_CASTERS_H_
#include <string>
#include <sstream>
#include <iomanip>
inline std::string GetMemoryString(size_t workset)
{
  std::stringstream ss;
  ss.precision(2);
  ss << std::fixed;

  if (workset >= 1073741824)
    ss << workset / 1073741824.f << " Gib";
  else if (workset >= 1048576)
    ss << workset / 1048576.f << " Mib";
  else if (workset >= 1024)
    ss << workset / 1024.f << " Kib";
  else ss << workset << " b";

  return ss.str();
}

inline std::string GetSpeedString(size_t workset)
{
  std::stringstream ss;
  ss.precision(2);
  ss << std::fixed;

  if (workset >= 1073741824)
    ss << workset / 1073741824.f << " Gib\\s";
  else if (workset >= 1048576)
    ss << workset / 1048576.f << " Mib\\s";
  else if (workset >= 1024)
    ss << workset / 1024.f << " Kib\\s";
  else ss << workset << " b\\s";

  return ss.str();
}
#endif //ORDER_CASTERS_H_
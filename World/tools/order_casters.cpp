#include "order_casters.h"
#include <iomanip>

std::string GetMemoryString(size_t workset)
{
  std::stringstream ss;

  if (workset > 1 << 30)
    ss << std::setprecision(2) << workset / float(1 << 30) << " Gib";
  else if (workset > 1 << 20)
    ss << std::setprecision(2) << workset / float(1 << 20) << " Mib";
  else if (workset > 1 << 11)
    ss << std::setprecision(2) << workset / float(1 << 11) << " Kib";
  else ss << workset << " b";

  return ss.str();
}

std::string GetSpeedString(size_t workset)
{
  std::stringstream ss;
  ss.precision(2);

  if (workset > 1 << 30)
    ss << std::setprecision(2) << workset / float(1 << 30) << " Gib\\s";
  else if (workset > 1 << 20)
    ss << std::setprecision(2) << workset / float(1 << 20) << " Mib\\s";
  else if (workset > 1 << 11)
    ss << std::setprecision(2) << workset / float(1 << 11) << " Kib\\s";
  else ss << workset << " b\\s";

  return ss.str();
}

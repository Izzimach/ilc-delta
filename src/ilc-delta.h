#pragma once

#include <vector>
#include <string>


#ifdef _WIN32
  #define ILC_DELTA_EXPORT __declspec(dllexport)
#else
  #define ILC_DELTA_EXPORT
#endif

ILC_DELTA_EXPORT void ilc_delta();
ILC_DELTA_EXPORT void ilc_delta_print_vector(const std::vector<std::string> &strings);

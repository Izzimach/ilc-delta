#pragma once

#include <vector>
#include <string>
#include <optional>

#ifdef _WIN32
  #define ILC_DELTA_EXPORT __declspec(dllexport)
#else
  #define ILC_DELTA_EXPORT
#endif

ILC_DELTA_EXPORT void ilc_delta();
ILC_DELTA_EXPORT void ilc_delta_print_vector(const std::vector<std::string> &strings);

template <typename T>
concept IsDeltaType = requires (T d1, T d2) {
  { d1 + d2 } -> std::same_as<T>;
  { d1.split() } -> std::same_as<std::optional<std::tuple<T,T>>>;
};

template <typename T>
concept IsXDXType =
 IsDeltaType<typename T::DeltaType> &&
 requires (T::ValueType a, T::ValueType b, T::DeltaType d) {
  { T::init(a) } -> std::same_as<T>;
  { T::diff(a,b) }-> std::same_as<typename T::DeltaType>;
  { a.patch(d) } -> std::same_as<T>;
  { a.complete() } -> std::same_as<typename T::ValueType>;
};


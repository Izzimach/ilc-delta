#pragma once

#include <vector>
#include <string>
#include <optional>

namespace ilc {

template <typename T>
concept IsDeltaType = requires (T d1, T d2) {
  { d1 + d2 } -> std::same_as<T>;
};

template <typename T>
concept IsXDXType =
 IsDeltaType<typename T::DeltaType> &&
 requires (T::ValueType a, T::ValueType b, T::DeltaType d, T x) {
  { T::init(a) } -> std::same_as<T>;
  { T::diff(a, b) }-> std::same_as<typename T::DeltaType>;
  { x.patch(d) } -> std::same_as<T>;
  { x.complete() } -> std::same_as<typename T::ValueType>;
};

} // namespace ilc
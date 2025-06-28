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
concept IsDeltaType = requires (T::ValueType a, T::ValueType b, T::DeltaType d) {
  { T::init(a) } -> std::same_as<T>;
  { T::diff(a,b) }-> std::same_as<typename T::DeltaType>;
  { a.patch(d) } -> std::same_as<T>;
  { a.complete() } -> std::same_as<typename T::ValueType>;
};



template <typename T>
struct Replaceable {
  using ValueType = T;
  using DeltaType = std::optional<T>;

  const T value_;
  const std::optional<T> delta_;

  static auto init(T value) -> T {
    return Replaceable<T>{ .value_ = value, .delta_ = std::nullopt };
  }

  auto patch(std::optional<T> new_delta) const -> Replaceable<T> {
    return {
      .value_ = this.value_,
      // we use the new delta if it exists, otherwise we keep the old delta
      .delta_ = new_delta ? new_delta : this.delta_};
  }

  auto diff(const T other) const  -> std::optional<T> {
    if (value_ == other.value_) {
      return std::nullopt;
    } else {
      return other.value_;
    }
  }

  auto complete() const -> T {
    if (delta_) {
      return *delta_;
    } else {
      return value_;
    }
  }
};


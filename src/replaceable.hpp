#include "ilc-delta.hpp"

namespace ilc {

template <typename T>
struct Replacing : public std::optional<T> {
    static auto no_diff() -> Replacing<T> {
        return {std::nullopt};
    }

    auto operator+(const Replacing<T>& other) const -> Replacing<T> {
        if (other.has_value()) {
            return {*other};
        }
        return *this;
    };

    auto split() const -> std::optional<std::tuple<Replacing<T>, Replacing<T>>> {
        // can't split Replacing<T>
        return std::nullopt;
    }
};

template <typename T>
struct Replaceable {
    using ValueType = T;
    using DeltaType = Replacing<T>;

    ValueType value_;
    DeltaType delta_;

    static auto init(const T& value) -> Replaceable<T> {
        return {
            .value_ = value, 
            .delta_ = std::nullopt
        };
    }

    auto patch(const Replacing<T>& delta) const -> Replaceable<T>{
        return {
            .value_ = this->value_,
            .delta_ = this->delta_ + delta
        };
    }
    
    T complete() const {
        if (delta_.has_value()) {
            return *delta_;
        }
        return value_;
    }

    static auto diff(const T& left, const T& right) -> Replacing<T> {
        if (left != right) {
            return {right};
        }
        return {std::nullopt};
    }
};

} // namespace ilc
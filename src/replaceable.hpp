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
    
    auto complete() const -> T{
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

    // standard way to modify a Replaceable: x.put(new_value)
    auto put(const T&& new_value) const -> Replaceable<T> {
        return patch(DeltaType{new_value});
    }
};

// predefined replaceable types
using r_float = Replaceable<float>;
using r_double = Replaceable<double>;
using r_int = Replaceable<int>;
using r_uint = Replaceable<unsigned int>;
using r_int16 = Replaceable<int16_t>;
using r_uint16 = Replaceable<uint16_t>;
using r_int8 = Replaceable<int8_t>;
using r_uint8 = Replaceable<uint8_t>;
using r_string = Replaceable<std::string>;

// predefined replacing types
using rp_float = Replacing<float>;
using rp_double = Replacing<double>;
using rp_int = Replacing<int>;
using rp_uint = Replacing<unsigned int>;
using rp_int16 = Replacing<int16_t>;
using rp_uint16 = Replacing<uint16_t>;
using rp_int8 = Replacing<int8_t>;
using rp_uint8 = Replacing<uint8_t>;
using rp_string = Replacing<std::string>;

} // namespace ilc
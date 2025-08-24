#pragma once

// ILC tuple type
// For any tuple <A,B> the delta is just a tuple of deltas: <dA,dB> 

#include "ilc-delta.hpp"

namespace ilc {

template <typename A, typename B>
requires IsXDXType<A> && IsXDXType<B>
struct Tuple {
    using ValueType = std::tuple<typename A::ValueType, typename B::ValueType>;
    using DeltaType = std::tuple<typename A::DeltaType, typename B::DeltaType>;

    ValueType value_;
    DeltaType delta_;

    static auto init(const ValueType& value) -> Tuple<A, B> {
        return {
            .value_ = value,
            .delta_ = std::make_tuple<typename A::DeltaType, typename B::DeltaType>({},{})
        };
    }

    auto patch(const DeltaType& delta) const -> Tuple<A, B> {
        return {
            .value_ = this->value_,
            .delta_ = std::make_tuple(
                std::get<0>(this->delta_) + std::get<0>(delta),
                std::get<1>(this->delta_) + std::get<1>(delta)
            )   
        };
    }

    auto complete() const -> ValueType{
        A a = A::init(std::get<0>(value_)).patch(std::get<0>(delta_));
        B b = B::init(std::get<1>(value_)).patch(std::get<1>(delta_));
        return {a.complete(), b.complete()};
    }

    static auto diff(const ValueType& left, const ValueType& right) -> DeltaType {
        typename A::DeltaType da = A::diff(std::get<0>(left),std::get<0>(right));
        typename B::DeltaType db = B::diff(std::get<1>(left),std::get<1>(right));
        return { da, db };
    }

    auto operator==(const Tuple<A, B>& other) const -> bool {
        return value_ == other.value_ && delta_ == other.delta_;
    }
};

} // namespace ilc
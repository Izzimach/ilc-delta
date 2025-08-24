#pragma once

#include "ilc-delta.hpp"
#include <immer/map.hpp>
#include <immer/map_transient.hpp>

namespace ilc {

template<typename T, typename C = unsigned int, typename D = int>
struct MultiSet {
    using ValueType = immer::map<T,C>;
    using DeltaType = immer::map<T,D>;
    using TransientDeltaType = immer::map_transient<T,D>;

    ValueType value_;
    DeltaType delta_;

    static auto init(const ValueType& value) -> MultiSet<T, C, D> {
        return {
            .value_ = value,
            .delta_ = {}
        };
    }

    static auto diff(const ValueType& left, const ValueType& right) -> DeltaType {
        TransientDeltaType delta;
        for (const auto& [key, count] : left) {
            if (right.find(key) == right.end()) {
                delta[key] = -C(count);
            } else {
                C diff_count = right.at(key) - count;
                if (diff_count != 0) {
                    delta[key] = D(diff_count);
                }
            }
        }
        for (const auto& [key, count] : right) {
            if (left.find(key) == left.end()) {
                delta[key] = C(count);
            }
        }
        return delta.persistent();
    }

    auto patch(const DeltaType& new_delta) const -> MultiSet<T, C, D> {
        TransientDeltaType transient_delta = delta_.transient();
        for (const auto& [key, change] : new_delta) {
            if (transient_delta.find(key) != transient_delta.end()) {
                transient_delta[key] += change;
            } else {
                transient_delta[key] = change;
            }
        }
        return {
            .value_ = value_,
            .delta_ = transient_delta.persistent()
        };
    }

    auto complete() const -> ValueType {
        auto complete_value = value_.transient();
        for (const auto& [key, change] : delta_) {
            if (change > 0) {
                complete_value.update(key, [=](auto c) { return c + change;});
            } else if (change < 0) {
                C complete_count = std::max(complete_value[key] + change, C(0));
                if (complete_count > 0) {
                    complete_value.set(key, complete_count);
                } else {
                    complete_value.erase(key);
                }
            }
        }
        return complete_value.persistent();
    }

    // typically you increment/decrement elements
    auto increment(const T& key, D count = 1) -> MultiSet<T, C, D> {
        TransientDeltaType transient_delta = delta_.transient();
        if (transient_delta.find(key) != nullptr) {
            transient_delta.update(key, [=](auto c) { return c + count;});
            if (transient_delta[key] == 0) {
                transient_delta.erase(key);
            }
        } else {
            transient_delta.set(key, count);
        }            
        return {
            .value_ = value_,
            .delta_ = transient_delta.persistent()
        };
    }

    auto decrement(const T& key, D count = 1) -> MultiSet<T, C, D> {
        TransientDeltaType transient_delta = delta_.transient();
        if (transient_delta.find(key) != nullptr) {
            transient_delta.update(key,[=](auto c) { return c - count;});
            if (transient_delta[key] == 0) {
                transient_delta.erase(key);
            }
        } else {
            transient_delta.set(key, -count);
        }
        return {
            .value_ = value_,
            .delta_ = transient_delta.persistent()
        };
    }

};

}
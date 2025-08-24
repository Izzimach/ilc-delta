#pragma once

#include "ilc-delta.hpp"
#include "multiset.hpp"

#include <immer/set.hpp>
#include <immer/set_transient.hpp>

namespace ilc {

template <typename T>
struct AddRemoveSet {
    immer::set<T> add_ = {};
    immer::set<T> remove_ = {};

    auto operator+(const AddRemoveSet<T>& other) const -> AddRemoveSet<T> {
        auto new_add = add_;
        auto new_remove = remove_;

        for (const auto& item : other.add_) {
            new_add = new_add.insert(item);
            new_remove = new_remove.erase(item);
        }

        for (const auto& item : other.remove_) {
            new_add.erase(item);
            new_remove.insert(item);
        }

        return {
            .add_ = new_add,
            .remove_ = new_remove
        };
    }

    auto add(const T& item) const -> AddRemoveSet<T> {
        return {
            .add_ = add_.insert(item),
            .remove_ = remove_.erase(item)
        };
    }

    auto remove(const T& item) const -> AddRemoveSet<T> {
        return {
            .add_ = add_.erase(item),
            .remove_ = remove_.insert(item)
        };
    }
};

template <typename T>
struct Set {
    using ValueType = immer::set<T>;
    using DeltaType = AddRemoveSet<T>;
    using TransientDeltaType = immer::set_transient<T>;

    ValueType value_;
    DeltaType delta_;

    static auto init(const immer::set<T>& value) -> Set<T> {
        return {
            .value_ = value,
            .delta_ = {}
        };
    }

    static auto diff(const immer::set<T>& left, const immer::set<T>& right) -> DeltaType {
        TransientDeltaType to_add;
        TransientDeltaType to_remove;

        for (const auto& item : left) {
            if (right.find(item) == right.end()) {
                to_remove = to_remove.insert(item);
            }
        }

        for (const auto& item : right) {
            if (left.find(item) == left.end()) {
                to_add = to_add.insert(item);
            }
        }

        return {
            .add_ = to_add.persistent(),
            .remove_ = to_remove.persistent()
        };
    }

    auto patch(const DeltaType& new_delta) const -> Set<T> {
        TransientDeltaType transient_delta = delta_.transient();
        for (const auto& item : new_delta.add_) {
            transient_delta = transient_delta.insert(item);
        }
        for (const auto& item : new_delta.remove_) {
            transient_delta = transient_delta.erase(item);
        }
        return {
            .value_ = value_,
            .delta_ = transient_delta.persistent()
        };
    }

    auto complete() const -> immer::set<T> {
        auto complete_value = value_.transient();
        for (const auto& item : delta_.add_) {  
            complete_value.insert(item);
        }
        for (const auto& item : delta_.remove_) {
            complete_value.erase(item);
        }
        return {complete_value.persistent()};
    }

    auto add(const T& item) -> Set<T> {
        auto new_delta = delta_.add(item);
        return {
            .value_ = value_,
            .delta_ = new_delta
        };
    }
    auto remove(const T& item) -> Set<T> {
        auto new_delta = delta_.remove(item);
        return {
            .value_ = value_,
            .delta_ = new_delta
        };
    }
};

template <typename T, typename C, typename D>
auto MultiSetToSet(const MultiSet<T,C,D>& ms) -> Set<T> {
    immer::set<T> base_set;
    for (const auto& [key, count] : ms.value_) {
        base_set = base_set.insert(key);
    }

    AddRemoveSet<T> delta_set;
    for (const auto& [key, change] : ms.delta_) {
        auto original_count = ms.value_.find(key);
        if (original_count == nullptr && change > 0) {
            // adding something to the set
            delta_set = delta_set.add(key);
        } else if (*original_count + change <= 0) {
            delta_set = delta_set.remove(key);
        }
    }

    return {
        .value_ = base_set,
        .delta_ = delta_set
    };
}

}
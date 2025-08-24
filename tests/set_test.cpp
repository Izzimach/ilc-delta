#include "set.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Set basic operations", "[basic][set]") {
    auto t1 = ilc::Set<int>::init(immer::set<int>{});
    auto t2 = t1.add(1).add(4).add(13);

    REQUIRE(t1.complete().empty());
    REQUIRE(t2.complete().find(1));
    REQUIRE(t2.complete().find(4));
    REQUIRE(t2.complete().find(13));
    REQUIRE(t2.complete().find(5) == nullptr); 

    auto t3 = t2.remove(4).remove(6);

    REQUIRE(t3.complete().find(4) == nullptr);
    REQUIRE(t3.complete().find(6) == nullptr);
}

TEST_CASE("Multset to set", "[conversion][set][multiset]") {
    auto t1 = ilc::MultiSet<int>::init(immer::map<int,unsigned int>{});
    auto t2 = t1.increment(1).increment(4).increment(1);

    auto t3 = ilc::MultiSetToSet(t2);
    REQUIRE(t3.complete().find(1));
    REQUIRE(t3.complete().find(4));
    REQUIRE(t3.complete().find(2) == nullptr);

    auto t4 = t2.decrement(4).decrement(1).decrement(1);
    auto t5 = ilc::MultiSetToSet(t4);
    REQUIRE(t5.complete().find(1) == nullptr);
    REQUIRE(t5.complete().find(4) == nullptr);
}

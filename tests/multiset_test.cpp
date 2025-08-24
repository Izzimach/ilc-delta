#include "multiset.hpp"

#include <immer/map.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Multiset basic operations", "[basic][multiset]") {
    auto t1 = ilc::MultiSet<int>::init(immer::map<int,unsigned int>{});
    auto t2 = t1.increment(1).increment(4).increment(1);

    REQUIRE(t1.complete().empty());
    REQUIRE(t2.complete().at(1) == 2);
    CHECK_THROWS(t2.complete().at(2) == 0); 
    REQUIRE(t2.complete().at(4) == 1); 

    auto t3 = t2.decrement(4).decrement(1);

    REQUIRE(t3.complete().at(1) == 1);
    CHECK_THROWS(t3.complete().at(4) == 0);
}

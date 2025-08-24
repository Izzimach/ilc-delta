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

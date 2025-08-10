
#include "replaceable.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Replaceable basic operations", "[replaceable]") {
    Replaceable<int> r1 = Replaceable<int>::init(10);
    Replaceable<int> r2 = r1.patch(Replacing<int>{20});

    REQUIRE(r1.complete() == 10);
    REQUIRE(r2.complete() == 20);

    auto diff = r1.diff(r2);
    REQUIRE(diff.has_value());
    REQUIRE(*diff == 20);

    auto r3 = r2.patch(Replacing<int>{30});
    REQUIRE(r3.complete() == 30);
}
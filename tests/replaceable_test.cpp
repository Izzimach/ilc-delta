
#include "replaceable.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Replaceable basic operations", "[basic][replaceable]") {
    const ilc::Replaceable<int> r1 = ilc::r_int::init(10);
    const ilc::Replaceable<int> r2 = r1.patch(ilc::rp_int{20});

    REQUIRE(r1.complete() == 10);
    REQUIRE(r2.complete() == 20);

    auto diff = ilc::r_int::diff(10,20);
    REQUIRE(diff.has_value());
    REQUIRE(*diff == 20);

    auto r3 = r2.patch(ilc::rp_int{30});
    REQUIRE(r3.complete() == 30);
}
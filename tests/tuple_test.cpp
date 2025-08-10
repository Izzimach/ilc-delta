#include "tuple.hpp"
#include "replaceable.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Tuple basic operations", "[basic][tuple]") {
    auto t1 = ilc::Tuple<ilc::Replaceable<int>,ilc::Replaceable<double>>::init({10,20.5});
    auto t2 = t1.patch({ilc::Replacing<int>{15}, ilc::Replacing<double>{25.5}});

    REQUIRE(std::get<0>(t1.complete()) == 10);
    REQUIRE(std::get<1>(t1.complete()) == 20.5);
    REQUIRE(std::get<0>(t2.complete()) == 15);
    REQUIRE(std::get<1>(t2.complete()) == 25.5);
 
    std::tuple<ilc::Replacing<int>,ilc::Replacing<double>> diff =
        ilc::Tuple<ilc::Replaceable<int>,ilc::Replaceable<double>>::diff({10,20.5},{15,25.5});
    REQUIRE(std::get<0>(diff).has_value());
    REQUIRE(*std::get<0>(diff) == 15);
    REQUIRE(std::get<1>(diff).has_value());
    REQUIRE(*std::get<1>(diff) == 25.5);
 
    auto t3 = t2.patch({ilc::Replacing<int>{20}, ilc::Replacing<double>{30.5}});
    REQUIRE(std::get<0>(t3.complete()) == 20);
    REQUIRE(std::get<1>(t3.complete()) == 30.5);
}

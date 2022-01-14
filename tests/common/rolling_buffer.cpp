#include <common/rolling_buffer.h>

#include <catch2/catch_all.hpp>

TEST_CASE("Creating rolling buffer", "[Common.rolling_buffer]") {
    auto buffer = let::rolling_buffer<uint32_t, 5>();
}

TEST_CASE("Inserting values (no overflow)", "[Common.rolling_buffer]") {
    auto buffer = let::rolling_buffer<uint32_t, 5>();

    for (auto i = 0; i < 3; i++)
        buffer.push_back(i);

    REQUIRE(buffer.size() == 3);

    for (auto i = 0;i < buffer.size(); i++)
        REQUIRE(i == buffer[i]);
}

TEST_CASE("Inserting values (overflow)", "[Common.rolling_buffer]") {
    auto buffer = let::rolling_buffer<uint32_t, 5>();

    for (auto i = 0; i < 10; i++)
        buffer.push_back(i);

    REQUIRE(buffer.size() == 5);

    for (auto i = 0; i < buffer.size(); i++)
        REQUIRE(i + 5 == buffer[i]);
}

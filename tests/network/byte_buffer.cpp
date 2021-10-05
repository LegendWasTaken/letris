#include <catch2/catch_all.hpp>

#include <network/byte_buffer.h>

TEST_CASE("Insertion", "[Network.byte_buffer]") {
    auto buffer = let::network::byte_buffer();
    GIVEN("An empty byte buffer with 0 capacity") {

        THEN("write 70 bytes") {
            for (auto i = 0; i < 70; i++)
                buffer.write_byte(std::byte(0));
        }
        REQUIRE(buffer.size() == 70);
    }
}

TEST_CASE("Reading", "[Network.byte_buffer]") {
    auto buffer = let::network::byte_buffer();

    for (auto i = 0; i < 64; i++)
        buffer.write_byte(std::byte(i));

    GIVEN("A byte buffer with 64 bytes") {
        WHEN("we read the first 20 bytes") {
            auto sum = 0;
            for (auto i = 0; i < 20; i++)
                sum += static_cast<int>(buffer.next_byte());

            REQUIRE(sum == 190);

            AND_THEN("we read the next 20 bytes") {
                sum = 0;
                auto bytes = buffer.next_bytes(20);
                for (auto i = 0; i < 20; i++)
                    sum += static_cast<int>(bytes[i]);
                REQUIRE(sum == 590);
            }
        }
    }
}

TEST_CASE("Capacity", "[Network.byte_buffer]") {
    GIVEN("A buffer with 5 bytes capacity") {
        auto buffer = let::network::byte_buffer(5);
        REQUIRE(buffer.size() == 0);
        REQUIRE(buffer.capacity() == 5);

        THEN("the buffer is resized to 40") {
            buffer.resize(40);

            REQUIRE(buffer.size() == 40);
            REQUIRE(buffer.capacity() == 40);

            AND_THEN("we reserve 512") {
                buffer.reserve(512);

                REQUIRE(buffer.size() == 40);
                REQUIRE(buffer.capacity() == 512);
            }
        }
    }

    AND_GIVEN("A buffer with 0 bytes of capacity") {
        auto buffer = let::network::byte_buffer();
        REQUIRE(buffer.size() == 0);
        REQUIRE(buffer.capacity() == 0);
    }
}

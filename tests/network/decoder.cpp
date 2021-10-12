#include <catch2/catch_all.hpp>

#include <common/network/decoder.h>
#include <common/network/encoder.h>

TEST_CASE("Decode-Int8", "[Network.decoder]") {
    auto buffer = let::network::byte_buffer();
    buffer.write_byte(std::byte(50));
    buffer.write_byte(std::byte(50));

    GIVEN("A buffer to decode") {
        THEN("decode an int8") {
            auto value = std::byte();
            let::network::decoder::read(buffer, value);
            REQUIRE(value == std::byte(50));

            AND_THEN("decode a uint8") {
                auto u8 = std::uint8_t();
                let::network::decoder::read(buffer, u8);
                REQUIRE(u8 == 50);
            }
        }
    }
}

TEST_CASE("Decode-Int16", "[Network.decoder]") {
    auto buffer = let::network::byte_buffer();
    buffer.write_byte(std::byte(0x02));
    buffer.write_byte(std::byte(0x00));
    buffer.write_byte(std::byte(0x7D));
    buffer.write_byte(std::byte(0x00));

    GIVEN("A buffer to decode") {
        THEN("decode an int16") {
            auto value = std::int16_t();
            let::network::decoder::read(buffer, value);
            REQUIRE(value == 512);

            AND_THEN("decode a uint16") {
                auto u16 = std::uint16_t();
                let::network::decoder::read(buffer, u16);
                REQUIRE(u16 == 32'000);
            }
        }
    }
}

TEST_CASE("Decode-Int32", "[Network.decoder]") {
    auto buffer = let::network::byte_buffer();
    buffer.write_byte(std::byte(0x02));
    buffer.write_byte(std::byte(0xF0));
    buffer.write_byte(std::byte(0xB5));
    buffer.write_byte(std::byte(0x4B));

    buffer.write_byte(std::byte(0xC4));
    buffer.write_byte(std::byte(0x63));
    buffer.write_byte(std::byte(0x1E));
    buffer.write_byte(std::byte(0xD5));

    GIVEN("A buffer to decode") {
        THEN("decode an int32") {
            auto value = std::int32_t();
            let::network::decoder::read(buffer, value);
            REQUIRE(value == 4'9329'483);

            AND_THEN("decode a uint32") {
                auto u32 = std::uint32_t();
                let::network::decoder::read(buffer, u32);
                REQUIRE(u32 == 3'294'830'293);
            }
        }
    }
}

TEST_CASE("Decode-Int64", "[Network.decoder]") {
    auto buffer = let::network::byte_buffer();
    buffer.write_byte(std::byte(0x00));
    buffer.write_byte(std::byte(0x00));
    buffer.write_byte(std::byte(0x04));
    buffer.write_byte(std::byte(0x66));
    buffer.write_byte(std::byte(0xF7));
    buffer.write_byte(std::byte(0x4A));
    buffer.write_byte(std::byte(0xC6));
    buffer.write_byte(std::byte(0xC9));

    buffer.write_byte(std::byte(0x0E));
    buffer.write_byte(std::byte(0x8C));
    buffer.write_byte(std::byte(0x6A));
    buffer.write_byte(std::byte(0x27));
    buffer.write_byte(std::byte(0x4F));
    buffer.write_byte(std::byte(0x6F));
    buffer.write_byte(std::byte(0x59));
    buffer.write_byte(std::byte(0xFC));

    GIVEN("A buffer to decode") {
        THEN("decode an int64") {
            auto value = std::int64_t();
            let::network::decoder::read(buffer, value);
            REQUIRE(value == 4'840'282'048'201);

            AND_THEN("decode a uint8") {
                auto u64 = std::uint64_t();
                let::network::decoder::read(buffer, u64);
                REQUIRE(u64 == 1'048'329'530'339'449'340);
            }
        }
    }
}

TEST_CASE("Decode-Float32", "[Network.decoder]") {
    auto buffer = let::network::byte_buffer();
    buffer.write_byte(std::byte(0x44));
    buffer.write_byte(std::byte(0x9a));
    buffer.write_byte(std::byte(0x50));
    buffer.write_byte(std::byte(0x00));

    GIVEN("A buffer to decode") {
        THEN("decode a float32") {
            auto value = float();
            let::network::decoder::read(buffer, value);
            REQUIRE(value == 1234.5);
        }
    }
}

TEST_CASE("Decode-Float64", "[Network.decoder]") {
    auto buffer = let::network::byte_buffer();
    buffer.write_byte(std::byte(0x40));
    buffer.write_byte(std::byte(0xC8));
    buffer.write_byte(std::byte(0x1C));
    buffer.write_byte(std::byte(0xD6));
    buffer.write_byte(std::byte(0xE6));
    buffer.write_byte(std::byte(0x31));
    buffer.write_byte(std::byte(0xF8));
    buffer.write_byte(std::byte(0xA1));

    GIVEN("A buffer to decode") {
        THEN("decode a float64") {
            auto value = double();
            let::network::decoder::read(buffer, value);
            REQUIRE(value == 12345.6789);
        }
    }
}

TEST_CASE("Decode-Varint", "[Network.decoder]") {
    auto buffer = let::network::byte_buffer();
    buffer.write_byte(std::byte(0xC8));
    buffer.write_byte(std::byte(0x9D));
    buffer.write_byte(std::byte(0x1E));

    GIVEN("A buffer to decode") {
        THEN("decode a varint") {
            auto value = let::var_int();
            let::network::decoder::read(buffer, value);
            REQUIRE(value.val == 495304);
        }
    }
}

TEST_CASE("Decode-Varlong", "[Network.decoder]") {
    auto buffer = let::network::byte_buffer();

    buffer.write_byte(std::byte(0xE1));
    buffer.write_byte(std::byte(0xEA));
    buffer.write_byte(std::byte(0xF5));
    buffer.write_byte(std::byte(0xD6));
    buffer.write_byte(std::byte(0x0E));
    GIVEN("A buffer to decode") {
        THEN("decode a varlong") {
            auto value = let::var_long();
            let::network::decoder::read(buffer, value);
            REQUIRE(value.val == 3940382049);
        }
    }
}

TEST_CASE("Decode-String", "[Network.decoder]") {
    auto buffer = let::network::byte_buffer();

    buffer.write_byte(std::byte(0x0C));
    buffer.write_byte(std::byte(0x48));
    buffer.write_byte(std::byte(0x65));
    buffer.write_byte(std::byte(0x6C));

    buffer.write_byte(std::byte(0x6C));
    buffer.write_byte(std::byte(0x6F));
    buffer.write_byte(std::byte(0x20));
    buffer.write_byte(std::byte(0x57));

    buffer.write_byte(std::byte(0x6F));
    buffer.write_byte(std::byte(0x72));
    buffer.write_byte(std::byte(0x6C));
    buffer.write_byte(std::byte(0x64));

    buffer.write_byte(std::byte(0x21));
    GIVEN("A buffer to decode") {
        THEN("decode a string") {
            auto string = std::string();
            let::network::decoder::read(buffer, string);
            REQUIRE(string == "Hello World!");
        }
    }
}

TEST_CASE("Decode-UUID", "[Network.decoder]") {
    auto buffer = let::network::byte_buffer();

    buffer.write_byte(std::byte(0x00));
    buffer.write_byte(std::byte(0x00));
    buffer.write_byte(std::byte(0x00));
    buffer.write_byte(std::byte(0x00));

    buffer.write_byte(std::byte(0x00));
    buffer.write_byte(std::byte(0x00));
    buffer.write_byte(std::byte(0x04));
    buffer.write_byte(std::byte(0xD2));

    buffer.write_byte(std::byte(0x00));
    buffer.write_byte(std::byte(0x00));
    buffer.write_byte(std::byte(0x00));
    buffer.write_byte(std::byte(0x00));

    buffer.write_byte(std::byte(0x00));
    buffer.write_byte(std::byte(0x00));
    buffer.write_byte(std::byte(0x16));
    buffer.write_byte(std::byte(0x2E));
    GIVEN("A buffer to decode") {
        THEN("decode a UUID") {
            const auto target = let::uuid(1234, 5678);
            auto value = let::uuid();
            let::network::decoder::read(buffer, value);
            REQUIRE(value.least_significant_bits() == target.least_significant_bits());
            REQUIRE(value.most_significant_bits() == target.most_significant_bits());
        }
    }
}

TEST_CASE("Decode-Location", "[Network.decoder]") {
    auto buffer = let::network::byte_buffer();

    buffer.write_byte(std::byte(0x00));
    buffer.write_byte(std::byte(0x00));
    buffer.write_byte(std::byte(0x04));
    buffer.write_byte(std::byte(0x40));

    buffer.write_byte(std::byte(0x0C));
    buffer.write_byte(std::byte(0x00));
    buffer.write_byte(std::byte(0x00));
    buffer.write_byte(std::byte(0x1F));
    GIVEN("A buffer to decode") {
        THEN("decode a Location") {
            auto value = glm::ivec3();
            let::network::decoder::read(buffer, value);
            REQUIRE(value.x == 17);
            REQUIRE(value.y == 3);
            REQUIRE(value.z == 31);
        }
    }
}

TEST_CASE("Decode-Angle", "[Network.decoder]") {
    auto buffer = let::network::byte_buffer();

    buffer.write_byte(std::byte(0x25));
    GIVEN("A buffer to decode") {
        THEN("decode a angle") {
            auto value = let::angle();
            let::network::decoder::read(buffer, value);
            REQUIRE(value.val == 37);
        }
    }
}

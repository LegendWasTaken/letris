#include <catch2/catch_all.hpp>

#include <network/encoder.h>

TEST_CASE("Encode-Int8", "[Network.encoder]") {
    auto buffer = let::network::byte_buffer();

    GIVEN("A buffer to encode") {
        THEN("encode an int8") {
            let::network::encoder::write(buffer, std::byte(-50));
            REQUIRE(buffer.next_byte() == std::byte(-50));
        }

        AND_THEN("encode a uint8") {
            let::network::encoder::write(buffer, uint8_t(200));
            REQUIRE(buffer.next_byte() == std::byte(200));
        }
    }
}

TEST_CASE("Encode-Int16", "[Network.encoder]") {
    auto buffer = let::network::byte_buffer();

    GIVEN("A buffer to encode") {
        THEN("encode an int16") {
            let::network::encoder::write(buffer, int16_t(512));
            REQUIRE(buffer.next_byte() == std::byte(0x02));
            REQUIRE(buffer.next_byte() == std::byte(0x00));
        }

        AND_THEN("encode a uint16") {
            let::network::encoder::write(buffer, uint16_t(32'000));
            REQUIRE(buffer.next_byte() == std::byte(0x7D));
            REQUIRE(buffer.next_byte() == std::byte(0x00));
        }
    }
}

TEST_CASE("Encode-Int32", "[Network.encoder]") {
    auto buffer = let::network::byte_buffer();

    GIVEN("A buffer to encode") {
        THEN("encode an int32") {
            let::network::encoder::write(buffer, int32_t( 4'9329'483));
            REQUIRE(buffer.next_byte() == std::byte(0x02));
            REQUIRE(buffer.next_byte() == std::byte(0xF0));
            REQUIRE(buffer.next_byte() == std::byte(0xB5));
            REQUIRE(buffer.next_byte() == std::byte(0x4B));
        }

        AND_THEN("encode a uint32") {
            let::network::encoder::write(buffer, uint32_t(3'294'830'293));
            REQUIRE(buffer.next_byte() == std::byte(0xC4));
            REQUIRE(buffer.next_byte() == std::byte(0x63));
            REQUIRE(buffer.next_byte() == std::byte(0x1E));
            REQUIRE(buffer.next_byte() == std::byte(0xD5));
        }
    }
}

TEST_CASE("Encode-Int64", "[Network.encoder]") {
    auto buffer = let::network::byte_buffer();

    GIVEN("A buffer to encode") {
        THEN("encode an int32") {
            let::network::encoder::write(buffer, int64_t(4'840'282'048'201));
            REQUIRE(buffer.next_byte() == std::byte(0x00));
            REQUIRE(buffer.next_byte() == std::byte(0x00));
            REQUIRE(buffer.next_byte() == std::byte(0x04));
            REQUIRE(buffer.next_byte() == std::byte(0x66));
            REQUIRE(buffer.next_byte() == std::byte(0xF7));
            REQUIRE(buffer.next_byte() == std::byte(0x4A));
            REQUIRE(buffer.next_byte() == std::byte(0xC6));
            REQUIRE(buffer.next_byte() == std::byte(0xC9));
        }

        AND_THEN("encode a uint32") {
            let::network::encoder::write(buffer, uint64_t(1'048'329'530'339'449'340));
            REQUIRE(buffer.next_byte() == std::byte(0x0E));
            REQUIRE(buffer.next_byte() == std::byte(0x8C));
            REQUIRE(buffer.next_byte() == std::byte(0x6A));
            REQUIRE(buffer.next_byte() == std::byte(0x27));
            REQUIRE(buffer.next_byte() == std::byte(0x4F));
            REQUIRE(buffer.next_byte() == std::byte(0x6F));
            REQUIRE(buffer.next_byte() == std::byte(0x59));
            REQUIRE(buffer.next_byte() == std::byte(0xFC));
        }
    }
}

TEST_CASE("Encode-Float32", "[Network.encoder]") {
    auto buffer = let::network::byte_buffer();

    GIVEN("A buffer to encode") {
        THEN("encode a float") {
            let::network::encoder::write(buffer, float(1234.5));
            REQUIRE(buffer.next_byte() == std::byte(0x44));
            REQUIRE(buffer.next_byte() == std::byte(0x9A));
            REQUIRE(buffer.next_byte() == std::byte(0x50));
            REQUIRE(buffer.next_byte() == std::byte(0x00));
        }
    }
}

TEST_CASE("Encode-Float64", "[Network.encoder]") {
    auto buffer = let::network::byte_buffer();

    GIVEN("A buffer to encode") {
        THEN("encode a double") {
            let::network::encoder::write(buffer, double(12345.6789));
            REQUIRE(buffer.next_byte() == std::byte(0x40));
            REQUIRE(buffer.next_byte() == std::byte(0xC8));
            REQUIRE(buffer.next_byte() == std::byte(0x1C));
            REQUIRE(buffer.next_byte() == std::byte(0xD6));
            REQUIRE(buffer.next_byte() == std::byte(0xE6));
            REQUIRE(buffer.next_byte() == std::byte(0x31));
            REQUIRE(buffer.next_byte() == std::byte(0xF8));
            REQUIRE(buffer.next_byte() == std::byte(0xA1));
        }
    }
}

TEST_CASE("Encode-Varint", "[Network.encoder]") {
    auto buffer = let::network::byte_buffer();

    GIVEN("A buffer to encode") {
        THEN("encode a varint") {
            let::network::encoder::write(buffer, let::var_int(495304));
            REQUIRE(buffer.next_byte() == std::byte(0xC8));
            REQUIRE(buffer.next_byte() == std::byte(0x9D));
            REQUIRE(buffer.next_byte() == std::byte(0x1E));
        }
    }
}

TEST_CASE("Encode-Varlong", "[Network.encoder]") {
    auto buffer = let::network::byte_buffer();

    GIVEN("A buffer to encode") {
        THEN("encode a varlong") {
            let::network::encoder::write(buffer, let::var_long(3940382049));
            REQUIRE(buffer.next_byte() == std::byte(0xE1));
            REQUIRE(buffer.next_byte() == std::byte(0xEA));
            REQUIRE(buffer.next_byte() == std::byte(0xF5));
            REQUIRE(buffer.next_byte() == std::byte(0xD6));
            REQUIRE(buffer.next_byte() == std::byte(0x0E));
        }
    }
}

TEST_CASE("Encode-String", "[Network.encoder]") {
    auto buffer = let::network::byte_buffer();

    GIVEN("A buffer to encode") {
        THEN("encode a string") {
            let::network::encoder::write(buffer, std::string("Hello World!"));
            REQUIRE(buffer.next_byte() == std::byte(0x0C));
            REQUIRE(buffer.next_byte() == std::byte(0x48));
            REQUIRE(buffer.next_byte() == std::byte(0x65));
            REQUIRE(buffer.next_byte() == std::byte(0x6C));

            REQUIRE(buffer.next_byte() == std::byte(0x6C));
            REQUIRE(buffer.next_byte() == std::byte(0x6F));
            REQUIRE(buffer.next_byte() == std::byte(0x20));
            REQUIRE(buffer.next_byte() == std::byte(0x57));

            REQUIRE(buffer.next_byte() == std::byte(0x6F));
            REQUIRE(buffer.next_byte() == std::byte(0x72));
            REQUIRE(buffer.next_byte() == std::byte(0x6C));
            REQUIRE(buffer.next_byte() == std::byte(0x64));

            REQUIRE(buffer.next_byte() == std::byte(0x21));
        }
    }
}

TEST_CASE("Encode-UUID", "[Network.encoder]") {
    auto buffer = let::network::byte_buffer();

    GIVEN("A buffer to encode") {
        THEN("encode a UUID") {
            let::network::encoder::write(buffer, let::uuid(1234, 5678));
            REQUIRE(buffer.next_byte() == std::byte(0x00));
            REQUIRE(buffer.next_byte() == std::byte(0x00));
            REQUIRE(buffer.next_byte() == std::byte(0x00));
            REQUIRE(buffer.next_byte() == std::byte(0x00));

            REQUIRE(buffer.next_byte() == std::byte(0x00));
            REQUIRE(buffer.next_byte() == std::byte(0x00));
            REQUIRE(buffer.next_byte() == std::byte(0x04));
            REQUIRE(buffer.next_byte() == std::byte(0xD2));

            REQUIRE(buffer.next_byte() == std::byte(0x00));
            REQUIRE(buffer.next_byte() == std::byte(0x00));
            REQUIRE(buffer.next_byte() == std::byte(0x00));
            REQUIRE(buffer.next_byte() == std::byte(0x00));

            REQUIRE(buffer.next_byte() == std::byte(0x00));
            REQUIRE(buffer.next_byte() == std::byte(0x00));
            REQUIRE(buffer.next_byte() == std::byte(0x16));
            REQUIRE(buffer.next_byte() == std::byte(0x2E));
        }
    }
}

TEST_CASE("Encode-Location", "[Network.encoder]") {
    auto buffer = let::network::byte_buffer();

    GIVEN("A buffer to encode") {
        THEN("encode a Location") {
            let::network::encoder::write(buffer, glm::ivec3(17, 3, 31));
            REQUIRE(buffer.next_byte() == std::byte(0x00));
            REQUIRE(buffer.next_byte() == std::byte(0x00));
            REQUIRE(buffer.next_byte() == std::byte(0x04));
            REQUIRE(buffer.next_byte() == std::byte(0x40));

            REQUIRE(buffer.next_byte() == std::byte(0x0C));
            REQUIRE(buffer.next_byte() == std::byte(0x00));
            REQUIRE(buffer.next_byte() == std::byte(0x00));
            REQUIRE(buffer.next_byte() == std::byte(0x1F));
        }
    }
}

TEST_CASE("Encode-Angle", "[Network.encoder]") {
    auto buffer = let::network::byte_buffer();

    GIVEN("A buffer to encode") {
        THEN("encode a Location") {
            let::network::encoder::write(buffer, let::angle(std::byte(37)));
            REQUIRE(buffer.next_byte() == std::byte(0x25));
        }
    }
}

#include <common/exception.h>

#include <catch2/catch_all.hpp>

// Todo: Write more exception testing code... Like that's ever happening
TEST_CASE("Source type name", "[Common.exception]") {
    auto file = __FILE__;
    auto line = __LINE__;

    let::exception(let::exception::source_type::network, "", file, line).where().type == "network";
    let::exception(let::exception::source_type::render, "", file, line).where().type == "render";
    let::exception(let::exception::source_type::window, "", file, line).where().type == "window";
    let::exception(let::exception::source_type::ui, "", file, line).where().type == "ui";
    let::exception(let::exception::source_type::physics, "", file, line).where().type == "physics";
    let::exception(let::exception::source_type::nbt, "", file, line).where().type == "nbt";
    let::exception(let::exception::source_type::opengl, "", file, line).where().type == "opengl";

}

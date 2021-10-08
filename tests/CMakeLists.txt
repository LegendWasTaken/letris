add_executable(letris_tests
        # Util
        util/files.cpp
        util/luts.cpp

        # UI
        ui/filesystem.cpp

        # Network
        network/byte_buffer.cpp
        network/decoder.cpp
        network/encoder.cpp

        common/exception.cpp)

Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.0.0-preview3
)

FetchContent_MakeAvailable(Catch2)

target_link_libraries(letris_tests PUBLIC Letris_lib Catch2::Catch2WithMain)
target_compile_definitions(letris_tests PUBLIC -DDATA_PATH="${CMAKE_SOURCE_DIR}/tests/data/")

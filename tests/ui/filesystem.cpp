#include <catch2/catch_all.hpp>

#include <ui/ultralight_filesystem.h>

TEST_CASE("File Handling", "[Ui.Filesystem]") {
    GIVEN("A filesystem") {
        auto filesystem = let::ultralight_filesystem();

        AND_WHEN("a file is opened") {
            auto handle = filesystem.OpenFile((std::string(DATA_PATH) + "loremipsum.txt").c_str(), true);
            REQUIRE(handle != ultralight::invalidFileHandle);

            THEN("reading the file") {
                auto file_size = std::int64_t();
                const auto valid_file = filesystem.GetFileSize(handle, file_size);
                REQUIRE(file_size == 1607);
                REQUIRE(valid_file);
                REQUIRE(file_size > 0);

                GIVEN("the file size") {
                    WHEN("reading the file") {
                        auto content = std::string();
                        content.resize(file_size);

                        filesystem.ReadFromFile(handle, content.data(), file_size);

                        // This is going to be broken on linux, ill deal with that once I'm there.
                        REQUIRE(content == "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Eget dolor morbi non arcu. Diam sollicitudin tempor id eu nisl nunc. Aliquet bibendum enim facilisis gravida neque convallis a cras. Cursus metus aliquam eleifend mi in nulla posuere sollicitudin. A arcu cursus vitae congue mauris. Urna nec tincidunt praesent semper feugiat. Non consectetur a erat nam at. Odio facilisis mauris sit amet massa vitae tortor. Sed pulvinar proin gravida hendrerit lectus. Sed lectus vestibulum mattis ullamcorper velit sed ullamcorper morbi tincidunt. Sit amet commodo nulla facilisi nullam vehicula ipsum a. Enim nunc faucibus a pellentesque sit amet porttitor eget dolor. Euismod lacinia at quis risus sed. Est pellentesque elit ullamcorper dignissim. Euismod quis viverra nibh cras. Aliquam eleifend mi in nulla posuere sollicitudin. Volutpat commodo sed egestas egestas fringilla phasellus. Nunc aliquet bibendum enim facilisis gravida neque convallis.\r\n\r\nScelerisque mauris pellentesque pulvinar pellentesque habitant morbi. Pellentesque pulvinar pellentesque habitant morbi tristique senectus et. Orci ac auctor augue mauris augue neque gravida in fermentum. Viverra suspendisse potenti nullam ac tortor vitae. Nunc vel risus commodo viverra maecenas accumsan lacus vel. Ut sem nulla pharetra diam sit. Elit pellentesque habitant morbi tristique. Aliquam vestibulum morbi blandit cursus risus. Pretium fusce id velit ut tortor pretium viverra suspendisse. Lacus luctus accumsan tortor posuere. Ornare lectus sit amet est placerat in egestas erat.");
                    }
                }
            }
            filesystem.CloseFile(handle);
        }
    }
}

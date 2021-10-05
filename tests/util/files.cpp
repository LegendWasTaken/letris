#include <catch2/catch_all.hpp>

#include <util/files.h>

TEST_CASE("Reading a file", "[Util.Files]") {
    GIVEN("2 paragraphs lorem ipsum text file") {
        WHEN("the file is read") {
            const auto content = let::read_file(std::string(DATA_PATH) + "loremipsum.txt");

            REQUIRE(content == "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Eget dolor morbi non arcu. Diam sollicitudin tempor id eu nisl nunc. Aliquet bibendum enim facilisis gravida neque convallis a cras. Cursus metus aliquam eleifend mi in nulla posuere sollicitudin. A arcu cursus vitae congue mauris. Urna nec tincidunt praesent semper feugiat. Non consectetur a erat nam at. Odio facilisis mauris sit amet massa vitae tortor. Sed pulvinar proin gravida hendrerit lectus. Sed lectus vestibulum mattis ullamcorper velit sed ullamcorper morbi tincidunt. Sit amet commodo nulla facilisi nullam vehicula ipsum a. Enim nunc faucibus a pellentesque sit amet porttitor eget dolor. Euismod lacinia at quis risus sed. Est pellentesque elit ullamcorper dignissim. Euismod quis viverra nibh cras. Aliquam eleifend mi in nulla posuere sollicitudin. Volutpat commodo sed egestas egestas fringilla phasellus. Nunc aliquet bibendum enim facilisis gravida neque convallis.\n"
                               "\n"
                               "Scelerisque mauris pellentesque pulvinar pellentesque habitant morbi. Pellentesque pulvinar pellentesque habitant morbi tristique senectus et. Orci ac auctor augue mauris augue neque gravida in fermentum. Viverra suspendisse potenti nullam ac tortor vitae. Nunc vel risus commodo viverra maecenas accumsan lacus vel. Ut sem nulla pharetra diam sit. Elit pellentesque habitant morbi tristique. Aliquam vestibulum morbi blandit cursus risus. Pretium fusce id velit ut tortor pretium viverra suspendisse. Lacus luctus accumsan tortor posuere. Ornare lectus sit amet est placerat in egestas erat.");
        }
    }
}

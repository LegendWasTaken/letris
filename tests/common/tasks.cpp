#include <catch2/catch_all.hpp>
#include <memory>

#include <common/task_executor.h>
#include <spdlog/spdlog.h>

TEST_CASE("Load string", "[Common.tasks]") {
    auto tasks = let::task_executor();

    auto resource = let::async_resource<std::string>();
    REQUIRE(resource.current_status() == let::async_resource<std::string>::status::prepared);

    auto task = tasks.load<std::string>([]{
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return std::string("Hello World!");
    }, &resource);

    task->wait();
    REQUIRE(resource.current_status() == let::async_resource<std::string>::status::ready);

    REQUIRE(*resource.get() == "Hello World!");
}

TEST_CASE("Load heap allocated string", "[Common.tasks]") {
    auto tasks = let::task_executor();

    auto resource = let::async_resource<std::unique_ptr<std::string>>();
    REQUIRE(resource.current_status() == let::async_resource<std::unique_ptr<std::string>>::status::prepared);

    auto task = tasks.load<std::unique_ptr<std::string>>([]{
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return std::make_unique<std::string>("Hello World!");
    }, &resource);

    task->wait();
    REQUIRE(resource.current_status() == let::async_resource<std::unique_ptr<std::string>>::status::ready);

    REQUIRE(*resource.get()->get() == "Hello World!");
}

TEST_CASE("Fail to load string", "[Common.tasks]") {
    auto tasks = let::task_executor();

    auto resource = let::async_resource<std::string>();
    REQUIRE(resource.current_status() == let::async_resource<std::string>::status::prepared);

    auto task = tasks.load<std::string>([]{
        std::this_thread::sleep_for(std::chrono::seconds(1));
        throw std::exception();
        return std::string("Hello World!");
    }, &resource);

    task->wait();
    REQUIRE(resource.current_status() == let::async_resource<std::string>::status::failed);

    REQUIRE_THROWS(resource.get());
}

TEST_CASE("Load callbacks", "[Common.tasks]") {
    auto tasks = let::task_executor();

    auto resource = let::async_resource<std::string>();
    REQUIRE(resource.current_status() == let::async_resource<std::string>::status::prepared);

    auto executed = false;

    auto task = tasks.load<std::string>([]{
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return std::string("Hello World!");
    }, &resource, {
        .fail = []{
            REQUIRE(false);
        },
        .load = [&executed]{
            executed = true;
        }
    });

    task->wait();
    REQUIRE(executed == true); // yes I know, == true
}

TEST_CASE("Fail callback", "[Common.tasks]") {
    auto tasks = let::task_executor();

    auto resource = let::async_resource<std::string>();
    REQUIRE(resource.current_status() == let::async_resource<std::string>::status::prepared);

    auto executed = false;

    // clang-format off
    auto task = tasks.load<std::string>([]{
        std::this_thread::sleep_for(std::chrono::seconds(1));
        throw std::exception();
        return std::string("Hello World!");
    }, &resource, {
        .fail = [&executed]{
            executed = true;
        },
        .load = []{
//            REQUIRE(false);
        }
    });
    // clang-format on

    task->wait();
    REQUIRE(executed == true); // yes I know, == true
}
#ifndef LETRIS_ASYNC_RESOURCE_H
#define LETRIS_ASYNC_RESOURCE_H

#include <atomic>
#include <memory>
#include <mutex>
#include <common/exception.h>

namespace let
{
    class task_executor;


    template <typename T>
    class async_resource {
    public:
        enum class status {
            prepared,
            loading,
            ready,
            failed
        };

        [[nodiscard]] status current_status() {
            return _status;
        }

        [[nodiscard]] T* get() {
            if (_status != status::ready)
                LET_EXCEPTION(let::exception::source_type::async, "Tried accessing a resource that was not ready");
            return &_resource;
        }

    private:

        friend task_executor;
        std::atomic<status> _status = status::prepared;

        T _resource;
    };
}

#endif //LETRIS_ASYNC_RESOURCE_H

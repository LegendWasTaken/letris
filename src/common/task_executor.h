#ifndef LETRIS_TASK_EXECUTOR_H
#define LETRIS_TASK_EXECUTOR_H

#include <functional>
#include <memory>

#include <common/async_resource.h>

namespace let {

    class load_task_handle {
    public:

        void wait();

    private:
        friend task_executor;
        std::condition_variable _done_cv;
        std::mutex _done_mutex;
    };

    template<typename T>
    class load_task {
    public:
        load_task(std::function<std::unique_ptr<T>()>, let::async_resource<T> &resource) {

        }

    private:


    };

    class task_executor {
    public:
        task_executor() = default;

        ~task_executor();

        task_executor(const task_executor &) = delete;

        task_executor(task_executor &&) = delete;

        task_executor &operator=(const task_executor &) = delete;

        task_executor &operator=(task_executor &&) = delete;

        template<typename T>
        [[nodiscard]] std::unique_ptr<load_task_handle>
        load(std::function<T()> func, let::async_resource<T> *resource) {
            auto handle = std::make_unique<load_task_handle>();

            _threads_in_flight.emplace_back([func, resource, handle = handle.get()] {
                resource->_status = async_resource<T>::status::loading;
                try {
                    resource->_resource = std::move(func());
                    resource->_status = async_resource<T>::status::ready;
                } catch (...) {
                    resource->_status = async_resource<T>::status::failed;
                }

                auto lock = std::lock_guard(handle->_done_mutex);
                handle->_done_cv.notify_one();
            });

            return std::move(handle);
        }

    private:
        std::vector<std::thread> _threads_in_flight;
    };
}


#endif //LETRIS_TASK_EXECUTOR_H

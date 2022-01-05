#include "thread_pool.h"

let::thread_pool::thread_pool(std::uint32_t thread_count)
{
    _threads.reserve(thread_count);
    for (auto i = 0; i < thread_count; i++)
    {
        _threads.emplace_back([this] { _thread_task(); });
    }
}

let::thread_pool::~thread_pool()
{
    {
        std::lock_guard lock(_work_lock);
        _tasks.push({});
    }
    _work_conditional.notify_all();
    for (auto &thread : _threads) thread.join();
}

void let::thread_pool::submit_task(const std::function<void()> &task)
{
    {
        std::lock_guard lock(_work_lock);
        _tasks.push(task);
    }

    _work_conditional.notify_one();
}

void let::thread_pool::submit_tasks(const std::vector<std::function<void()>> &tasks)
{
    {
        std::lock_guard lock(_work_lock);
        for (const auto &task : tasks) _tasks.push(task);
    }

    _work_conditional.notify_all();
}

std::optional<std::function<void()>> let::thread_pool::_next_task()
{
    std::unique_lock lock(_work_lock);
    if (_tasks.empty())
        _job_finished_conditional.notify_all();

    while (_tasks.empty()) _work_conditional.wait(lock);

    if (_tasks.front())
    {
        auto task = _tasks.front();
        _tasks.pop();
        return task;
    }

    return {};
}

void let::thread_pool::_thread_task()
{
    while (auto task = _next_task()) task.value()();
}

void let::thread_pool::flush()
{
    auto guard = std::unique_lock(_work_lock);

    do {
        _job_finished_conditional.wait(guard);
    } while (!_tasks.empty());
}
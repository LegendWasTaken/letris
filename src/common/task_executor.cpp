#include "task_executor.h"

void let::load_task_handle::wait() {
    auto lock = std::unique_lock(_done_mutex);
    _done_cv.wait(lock);
}

let::task_executor::~task_executor() {
    // tbh, waiting on all of them to finish isn't the worst idea...
    for (auto &thread : _threads_in_flight)
        thread.join();
}

#ifndef LETRIS_TIMER_H
#define LETRIS_TIMER_H

#include <array>
#include <limits>
#include <chrono>

namespace let {

    class timer {
    public:
        timer();

        void frame_start();

        void frame_end();

        [[nodiscard]] double time_since_start() const noexcept;

        [[maybe_unused]] [[nodiscard]] double average_frame_time() const noexcept;

        [[nodiscard]] double since_last_frame() const noexcept;

        double fastest_frame = std::numeric_limits<double>::max();

        double slowest_frame = std::numeric_limits<double>::min();

        std::array<double, 50> past_frame_times{};

    private:
        using Duration = std::chrono::duration<double, std::milli>;

        std::chrono::time_point<std::chrono::high_resolution_clock> start_time;

        std::chrono::time_point<std::chrono::high_resolution_clock> frame_start_time;

        std::chrono::time_point<std::chrono::high_resolution_clock> frame_end_time;
    };
}

#endif //LETRIS_TIMER_H

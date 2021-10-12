#ifndef LETRIS_LOCKED_RESOURCE_H
#define LETRIS_LOCKED_RESOURCE_H

#include <thread>
#include <mutex>

namespace let {

    template <typename T>
    class locked_resource {
    public:

        class resource_accessor {
        private:
            std::mutex *_access_mutex;
            T *_val;
        public:
            explicit resource_accessor(std::mutex *access_mutex, T &val) :
                _access_mutex(access_mutex), _val(&val) {
                _access_mutex->lock();
            }

            ~resource_accessor() {
                _access_mutex->unlock();
            }

            resource_accessor(const resource_accessor &) = delete;

            resource_accessor(resource_accessor &&) = delete;

            resource_accessor& operator=(const resource_accessor &) = delete;

            resource_accessor& operator=(resource_accessor&&) = delete;


            [[nodiscard]] explicit operator T&() {
                return *_val;
            }

            [[nodiscard]] explicit operator const T&() const {
                return *_val;
            }
        };

        template <class... Args>
        explicit locked_resource(Args&&... args) {
            _val = T(std::forward<Args>(args)...);
        }

        [[nodiscard]] resource_accessor value() {
            return resource_accessor(&_mutex, _val);
        }

    private:
        std::mutex _mutex;
        T _val;
    };
}

#endif //LETRIS_LOCKED_RESOURCE_H

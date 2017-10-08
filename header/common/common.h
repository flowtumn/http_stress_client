#ifndef COMMON_H_INCLUDE__
#define COMMON_H_INCLUDE__

#include <string>
#include "socket/socket.h"

namespace TrainingTask {

    template <typename T>
    auto toByteBuf(T&& v) {
        TrainingTask::ByteBuf r{};
        for (auto each : v) {
            r.emplace_back(each);
        }
        return r;
    }

    inline auto toString() { return std::string{}; }
    inline auto toString(std::string v) { return v; }
    inline auto toString(const char* p) {
        return std::string{ p };
    }

    /**
     * T Type to std::string
     */
    template <typename Head, typename ... Args>
    auto toString(Head&& head, Args&&... args) {
        return toString(head) + toString(std::forward <Args>(args)...);
    }

}

#endif //COMMON_H_INCLUDE__

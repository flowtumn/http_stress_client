#ifndef HTTP_STRESS_CLIENT_H_INCLUDE__
#define HTTP_STRESS_CLIENT_H_INCLUDE__

#include <memory>
#include "socket/isocket.h"

namespace TrainingTask {
    struct StressObserver {
        bool connection;
        int connectionError;
        int responseCode;
    };

    template <typename _Socket>
    class HttpStressClient {
        using F = std::function <bool()>;
        using Observer = std::function <void(int)>;
        
        static_assert(
                ::std::is_base_of <typename _Socket::value_type, TrainingTask::ISocket>::value
            ,   "_Socket must be a descendant of ISocket."
        );

    public:
        HttpStressClient() = default;
        ~HttpStressClient() = default;

        int64_t doStress(Observer observer) {
        }

        void stop() {
            //Push to stop function
            this->queue_.push([] { return true; });
        }

    private:
        std::unique_ptr <_Socket> socket_;
    };

};

#endif HTTP_STRESS_CLIENT_H_INCLUDE__

#ifndef HTTP_STRESS_CLIENT_H_INCLUDE__
#define HTTP_STRESS_CLIENT_H_INCLUDE__

#include <atomic>
#include <cstdint>
#include "socket/isocket.h"

namespace TrainingTask {
    class HttpStressClient {
    public:
        HttpStressClient() = default;
        ~HttpStressClient() {
            this->stop();
        }

        void doStress(const std::string& host, int port, const std::string& query);

        void clear() {
            this->total_    = 0;
            this->failuer_  = 0;
            this->error_    = 0;
            this->success_  = 0;
        }

         int64_t total() const { return this->total_; }
         int64_t failuer() const { return this->failuer_; }
         int64_t error() const { return this->error_; }
         int64_t success() const { return this->success_; }
                        
        void stop() {
            this->running_.store(false);
        }

    private:
        std::atomic <bool> running_{false};
        int64_t total_{0};
        int64_t failuer_{0};
        int64_t error_{0};
        int64_t success_{0};
    };

};

#endif //HTTP_STRESS_CLIENT_H_INCLUDE__

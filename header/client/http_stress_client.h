#ifndef HTTP_STRESS_CLIENT_H_INCLUDE__
#define HTTP_STRESS_CLIENT_H_INCLUDE__

#include <memory>
#include "socket/isocket.h"

template <typename _Socket>
class HttpStressClient {
    static_assert(
            ::std::is_base_of <typename _Socket::value_type, TrainingTask::ISocket>::value
        ,   "_Socket must be a descendant of ISocket."
    );

public:
    HttpStressClient() = default;
    ~HttpStressClient() = default;

private:
    std::unique_ptr <_Socket> socket_;
};

#endif HTTP_STRESS_CLIENT_H_INCLUDE__

﻿#ifndef HTTP_CLIENT_H_INCLUDE__
#define HTTP_CLIENT_H_INCLUDE__

#include <memory>
#include <type_traits>
#include "common/common.h"
#include "socket/isocket.h"

namespace flowTumn {
    template <typename _Socket>
    class HttpClient {
        static_assert(
                ::std::is_base_of <flowTumn::ISocket, _Socket>::value
            ,   "_Socket must be a descendant of ISocket."
        );

    public:
        HttpClient() = default;
        ~HttpClient() = default;

        auto doGet(const std::string& host, int port, const std::string& query) const {
            auto s = std::make_unique <_Socket> ();

            if (!s->initialize()) {
                throw std::runtime_error{"doGet initialize error."};
            }

            if (!s->connect(host, port)) {
                throw std::runtime_error{"doGet connect error."};
            }

            auto req = this->toGetRequest(host, query);
            if (static_cast <int> (req.size()) != s->write(flowTumn::toByteBuf(req))) {
                throw std::runtime_error{"doGet write error."};
            }

            return s;
        }

    private:
        constexpr std::string toGetRequest(const std::string& host, const std::string& query) const {
            return flowTumn::toString(
                "GET ", query, " ", "HTTP/1.1",
                "\r\n", "Host: ", host,
                "\r\n\r\n"
            );
        }
    };
};

#endif //HTTP_CLIENT_H_INCLUDE__


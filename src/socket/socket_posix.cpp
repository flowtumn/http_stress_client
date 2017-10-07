#include "socket/socket_posix.h"

#ifdef __linux__

#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

using SOCKET = int;
using namespace TrainingTask;

class SocketPosix::impl {
friend class SocketPosix;

public:
    impl() = default;
    ~impl() {
        this->shutdown();
    }

    void shutdown() {
        this->closeSocket();
    }

    bool initialize() {
        auto s = ::socket(AF_INET, SOCK_STREAM, 0);
        this->socket_ = std::make_unique <SOCKET>(s);
        return true;
    }

    bool closeSocket();

    SOCKET rowSocket() const {
        return this->socket_
            ? *this->socket_
            : 0;
    }

    template <typename FS, typename FE>
    auto socketControll(FS&& f, FE&& fe) {
        if (auto s = this->rowSocket()) {
            return f(s);
        } else {
            return fe();
        }
    }

private:
    std::unique_ptr <SOCKET> socket_;
};

//header内で実装すると、g++ビルドエラー
bool SocketPosix::impl::closeSocket() {
    return this->socketControll(
        [this](auto s) {
            //unique_ptrを破棄。
            this->socket_.reset();
            return 0 == ::close(s);
        },
        [] {
            return false;
        }
   );
}

SocketPosix::SocketPosix()
    : impl_(std::make_unique <SocketPosix::impl>())
{}

SocketPosix::~SocketPosix() = default;

bool SocketPosix::initialize() {
    return this->impl_->initialize();
}

ByteBuf SocketPosix::read(std::size_t bytes) {
    return this->impl_->socketControll(
        [bytes](auto s) {
            ByteBuf r(bytes);
            auto recvBytes = ::recv(s, &r[0], bytes, 0);
            r.resize(recvBytes);
            return r;
        },
        [] {
            return ByteBuf{};
        }
    );
}

int SocketPosix::write(const ByteBuf& data) {
    return this->impl_->socketControll(
        [&data](auto s) {
            return ::send(s, data.data(), data.size(), 0);
    },
        [] {
        return ssize_t{-1};
    }
    );
}

void SocketPosix::close() {
    this->impl_->closeSocket();
}

bool SocketPosix::connect(const std::string& host, int port) {
    if (auto byName = ::gethostbyname(host.data())) {
        return this->impl_->socketControll(
            [&byName, &host, port](auto s) {
                struct sockaddr_in info;

                info.sin_family = AF_INET;
                info.sin_port = ::htons(port);
                info.sin_addr.s_addr = *(unsigned int *)byName->h_addr_list[0];

                return 0 == ::connect(s, (struct sockaddr *)&info, sizeof(info));
            },
            [] {
                return false;
            }
        );
    }
    return false;
}

#endif


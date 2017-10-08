#include "socket/socket_win.h"

#ifdef _WIN32

#include <WinSock2.h>

using namespace TrainingTask;

class SocketWin::impl {
friend class SocketWin;

public:
    impl() = default;
    ~impl() {
        this->shutdown();
    }

    void shutdown() {
        this->closeSocket();
    }

    bool initialize() {
        WSADATA wsa;;
        if (0 != WSAStartup(MAKEWORD(2, 0), &wsa)) {
            return false;
        }

        auto s = ::socket(AF_INET, SOCK_STREAM, 0);
        this->socket_ = std::make_unique <SOCKET>(s);
        return true;
    }

    bool closeSocket() {
        return this->socketControll(
            [this](auto s) {
                //unique_ptrを破棄。
                this->socket_.reset();
                ::closesocket(s);
                return true;
            },
            [] {
                return false;
            }
        );
    }

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

SocketWin::SocketWin()
    : impl_(std::make_unique <SocketWin::impl>())
{}

SocketWin::~SocketWin() = default;

bool SocketWin::initialize() {
    return this->impl_->initialize();
}

ByteBuf SocketWin::read(std::size_t bytes) {
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

int SocketWin::write(const ByteBuf& data) {
    return this->impl_->socketControll(
        [&data](auto s) {
            return ::send(s, data.data(), data.size(), 0);
    },
        [] {
        return -1;
    }
    );
}

void SocketWin::close() {
    this->impl_->closeSocket();
}

bool SocketWin::connect(const std::string& host, int port) {
    if (auto byName = ::gethostbyname(host.data())) {
        return this->impl_->socketControll(
            [&byName, &host, port](auto s) {
                struct sockaddr_in info;

                info.sin_family = AF_INET;
                info.sin_port = static_cast <USHORT> (::htons(port) & 0xFFFF);
                info.sin_addr.S_un.S_addr = *((unsigned long *)(byName->h_addr));

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

#ifndef SOCKET_WIN_H_INCLUDE__
#define SOCKET_WIN_H_INCLUDE__

#include <memory>
#include "isocket.h"

namespace flowTumn {
    /**
     * Windows向けのSocketTCP-class
     */
    class SocketWin : public flowTumn::ISocketTCP {
    public:
        SocketWin();
        ~SocketWin();

        bool initialize() override;
        ByteBuf read(std::size_t bytes) override;
        int write(const ByteBuf& data) override;
        void close() override;
        bool connect(const std::string& host, int port) override;

    private:
        class impl;
        std::unique_ptr <impl> impl_;
    };
}

#endif //SOCKET_WIN_H_INCLUDE__

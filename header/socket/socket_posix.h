#ifndef SOCKET_POSIX_H_INCLUDE__
#define SOCKET_POSIX_H_INCLUDE__

#include <memory>
#include "isocket.h"

namespace TrainingTask {
    /**
     * PosixŒü‚¯‚ÌSocketTCP-class
     */
    class SocketPosix : public TrainingTask::ISocketTCP {
    public:
        SocketPosix();
        ~SocketPosix();

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

#endif //SOCKET_POSIX_H_INCLUDE__

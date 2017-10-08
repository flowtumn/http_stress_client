#ifndef I_SOCKET_H_INCLUDE__
#define I_SOCKET_H_INCLUDE__

#include <string>
#include <vector>

namespace flowTumn {
    using Byte = char;
    using ByteBuf = std::vector <Byte>;

    /**
     * Define socket interface.
     */
    class ISocket {
    public:
        virtual ~ISocket() = default;
        virtual bool initialize() = 0;
        virtual ByteBuf read(std::size_t bytes) = 0;
        virtual int write(const ByteBuf& data) = 0;
        virtual void close() = 0;
    };

    class ISocketTCP : public ISocket {
    public:
        virtual bool connect(const std::string& host, int port) = 0;
    };
}

#endif //SOCKET_H_INCLUDE__
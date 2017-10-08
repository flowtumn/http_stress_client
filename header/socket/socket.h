#ifndef SOCKET_H_INCLUDE__
#define SOCKET_H_INCLUDE__

#ifdef _WIN32
    #include "socket_win.h"
    namespace flowTumn {
        using Socket = flowTumn::SocketWin;
    };
#elif __linux__
    #include "socket_posix.h"
    namespace flowTumn {
        using Socket = flowTumn::SocketPosix;
    };
#else
    #error "Unsupport platform."
#endif

#endif //SOCKET_H_INCLUDE__
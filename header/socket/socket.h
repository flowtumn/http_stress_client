#ifndef SOCKET_H_INCLUDE__
#define SOCKET_H_INCLUDE__

#ifdef _WIN32
    #include "socket_win.h"
    namespace TrainingTask {
        using Socket = TrainingTask::SocketWin;
    };
#elif __linux__
    #include "socket_posix.h"
    namespace TrainingTask {
        using Socket = TrainingTask::SocketPosix;
    };
#else
    #error "Unsupport platform."
#endif

#endif //SOCKET_H_INCLUDE__
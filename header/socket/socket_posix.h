#ifndef SOCKET_POSIX_H_INCLUDE__
#define SOCKET_POSIX_H_INCLUDE__

#ifdef _WIN32
    #include "socket_win.h"
#elif __linux__
    #include "socket_posix.h"
#else
    #error "Unsupport platform."
#endif

#endif //SOCKET_POSIX_H_INCLUDE__
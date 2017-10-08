cmake_minimum_required(VERSION 3.1.0)

project(tcp_get)

set(CMAKE_CXX_STANDARD 14)

# Check for linux.
if ("${CMAKE_SYSTEM}" MATCHES "^Linux*")
  set(LINUX true)
endif()

if(APPLE)
  message("on Mac >>>")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
endif()

if (LINUX)
  message("on Linux >>>")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
  set(
    EXTRA_LIBS
    pthread
  )
endif()

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()

  add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)

  set(
    EXTRA_LIBS
    ws2_32.lib
  )
endif()

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/header)

set(
    SOURCES
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/client/http_client.cpp
    ${PROJECT_SOURCE_DIR}/src/client/http_stress_client.cpp
    ${PROJECT_SOURCE_DIR}/src/common/common.cpp
    ${PROJECT_SOURCE_DIR}/src/socket/isocket.cpp
    ${PROJECT_SOURCE_DIR}/src/socket/socket.cpp
    ${PROJECT_SOURCE_DIR}/src/socket/socket_win.cpp
    ${PROJECT_SOURCE_DIR}/src/socket/socket_posix.cpp
)

set (
    HEADERS
    ${PROJECT_SOURCE_DIR}/header/client/http_client.h
    ${PROJECT_SOURCE_DIR}/header/client/http_stress_client.h
    ${PROJECT_SOURCE_DIR}/header/common/common.h
    ${PROJECT_SOURCE_DIR}/header/socket/isocket.h
    ${PROJECT_SOURCE_DIR}/header/socket/socket.h
    ${PROJECT_SOURCE_DIR}/header/socket/socket_win.h
    ${PROJECT_SOURCE_DIR}/header/socket/socket_posix.h
)

add_executable(
    ${PROJECT_NAME}
    ${APP_BUNDLE}
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(
    ${PROJECT_NAME}
    LINK_PUBLIC
    ${EXTRA_LIBS}
)


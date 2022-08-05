PROJECT( core_net_lib )

cmake_minimum_required(VERSION 3.10)

SET(CMAKE_CXX_STANDARD 17)
add_definitions("-Wall -Wsign-conversion -Wextra -Weffc++")

if(COUPON_GCC_WARNING_AS_ERROR)
  add_definitions("-Werror")
endif()

SET( SRCS
    sock_interface.cpp
  )

INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

ADD_LIBRARY ( core_net_lib  SHARED
  ${SRCS}
  )


install(TARGETS core_net_lib LIBRARY DESTINATION ${LIB_INSTALL_DIR} ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
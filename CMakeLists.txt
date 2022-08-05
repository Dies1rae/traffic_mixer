PROJECT( traffic_mixer )

cmake_minimum_required (VERSION 3.10)
SET(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions("-Wall -Wsign-conversion -Wextra -Weffc++")

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic)
endif()

#SET(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_INSTALL_PREFIX}/share/cmake)
SET(cmake_utils ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Log4cpp REQUIRED)
find_package(Threads REQUIRED)

message(traffic_mixer: LOG4CPP_INCLUDE_DIR=${LOG4CPP_INCLUDE_DIR})
message(traffic_mixer: LOG4CPP_LIBRARIES=${LOG4CPP_LIBRARIES})
message(traffic_mixer: EXECUTABLE_OUTPUT_PATH=${EXECUTABLE_OUTPUT_PATH})

if (NOT (DEFINED EXECUTABLE_OUTPUT_PATH) OR (EXECUTABLE_OUTPUT_PATH STREQUAL ""))
  SET (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
endif ()
if (NOT (DEFINED LIBRARY_OUTPUT_PATH) OR (LIBRARY_OUTPUT_PATH STREQUAL ""))
  SET (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib${LIBSUFFIX})
endif ()
if (NOT (DEFINED BINARY_OUTPUT_PATH) OR (BINARY_OUTPUT_PATH STREQUAL ""))
  SET (BINARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib${LIBSUFFIX})
endif ()


SET(SRCS
  main.cpp
  traffic_core.cpp
  )

SET(LIBS
  ${SYSLIBS}
  ${LOG4CPP_LIBRARIES}
  core_net_lib
)

add_subdirectory(sock_interface)

INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/
  ${CMAKE_CURRENT_SOURCE_DIR}/sock_interface
  ${LOG4CPP_INCLUDE_DIR}
)


ADD_EXECUTABLE(traffic_mixer
  ${SRCS}
)

message(PROJECT_BINARY_DIR=${PROJECT_BINARY_DIR})

TARGET_LINK_LIBRARIES(traffic_mixer
  ${LIBS}
  Threads::Threads
)

INSTALL(TARGETS traffic_mixer DESTINATION bin)

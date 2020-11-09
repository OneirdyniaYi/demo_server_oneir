cmake_minimum_required(VERSION 3.1)
project(demo)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
set(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")

find_package(Protobuf)
if(Protobuf_FOUND)
    include_directories(${Protobuf_INCLUDE_DIRS})
endif()

include_directories(.)
include_directories(${PROJECT_SOURCE_DIR}/common
		    ${PROJECT_SOURCE_DIR}/build)
set(LIB_SRC
	common/buffer.cc
	common/epoll.cc
	common/msghead.cc
	common/socket.cc
    )

set(PROTO_FILES protosrc/Demo.proto protosrc/ProtoID.proto)
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

set(PROTOLIB ${PROTO_SRCS})

add_library(demo_lib ${LIB_SRC} ${PROTOLIB})
#add_library(Hook_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES (Hook_static PROPERTIES OUTPUT_NAME "Hook_lib")

set(SERVER_LIB
	server/server.cc 
	server/player.cc
)


add_executable(server server/main.cc ${SERVER_LIB})
target_link_libraries(server demo_lib ${Protobuf_LIBRARIES})

set(CLIENT_LIB 
	client/client.cc
)

add_executable(client client/main.cc ${CLIENT_LIB})
target_link_libraries(client demo_lib ${Protobuf_LIBRARIES})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

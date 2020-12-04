cmake_minimum_required(VERSION 3.1)
project(demo)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
#set(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")

find_package(Protobuf)
if(Protobuf_FOUND)
    include_directories(${Protobuf_INCLUDE_DIRS})
endif()


find_path(HIREDIS_HEADER hiredis)
#target_include_directories(redislib PUBLIC ${HIREDIS_HEADER})

find_library(HIREDIS_LIB hiredis)
#target_link_libraries(redislib ${HIREDIS_LIB})

find_path(REDIS_PLUS_PLUS_HEADER sw)
#target_include_directories(redislib PUBLIC ${REDIS_PLUS_PLUS_HEADER})

find_library(REDIS_PLUS_PLUS_LIB redis++)
#target_link_libraries(redislib ${REDIS_PLUS_PLUS_LIB})


find_path(MYSQL_HEADER mysql)

link_directories(/usr/lib64/mysql/)

include_directories(.)
include_directories(${PROJECT_SOURCE_DIR}/common
		    ${PROJECT_SOURCE_DIR}/build
			${PROJECT_SOURCE_DIR}/loginserver
			${REDIS_PLUS_PLUS_HEADER}
			${MYSQL_HEADER}
			)

set(LIB_SRC
	common/buffer.cc
	common/epoll.cc
	common/msghead.cc
	common/socket.cc
	common/mysqlplus.cc
	common/redisplus.cc
	common/clientmoudle.cc
    )

set(PROTO_FILES protosrc/Demo.proto protosrc/ProtoID.proto)
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

set(PROTOLIB ${PROTO_SRCS})

add_library(demo_lib SHARED ${LIB_SRC} ${PROTOLIB} )
#add_library(Hook_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES (Hook_static PROPERTIES OUTPUT_NAME "Hook_lib")

set(GAMESERVER_LIB
	gameserver/item.cc
	gameserver/backpack.cc
	gameserver/player.cc
	gameserver/server.cc

)


add_executable(gameserver gameserver/main.cc ${GAMESERVER_LIB} )
target_link_libraries(gameserver demo_lib ${Protobuf_LIBRARIES} ${REDIS_PLUS_PLUS_LIB} ${HIREDIS_LIB} mysqlclient)

set(LOGINSERVER_LIB
	loginserver/loginlogic.cc
	loginserver/loginserver.cc
)


add_executable(loginserver loginserver/main.cc ${LOGINSERVER_LIB} )
target_link_libraries(loginserver demo_lib ${Protobuf_LIBRARIES} ${REDIS_PLUS_PLUS_LIB} ${HIREDIS_LIB} mysqlclient)


set(GATESERVER_LIB
	gateserver/gatelogic.cc
	gateserver/gateserver.cc
)
add_executable(gateserver gateserver/main.cc ${GATESERVER_LIB} )
target_link_libraries(gateserver demo_lib ${Protobuf_LIBRARIES} ${REDIS_PLUS_PLUS_LIB} ${HIREDIS_LIB} mysqlclient)
#set(CLIENT_LIB 
#	client/client.cc
#)

#add_executable(client client/main.cc ${CLIENT_LIB})
#target_link_libraries(client demo_lib ${Protobuf_LIBRARIES})



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

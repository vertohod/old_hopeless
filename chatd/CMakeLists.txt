cmake_minimum_required(VERSION 2.8)

project(chatd)

find_package(Git REQUIRED)

execute_process(
    COMMAND ${GIT_EXECUTABLE} log HEAD~1..HEAD --pretty=oneline
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_HASH_TEMP
)

string(REGEX MATCH [0-9a-f]+ GIT_HASH "${GIT_HASH_TEMP}")

add_definitions(-DDAEMON=\"chatd\" -DGIT_HASH=\"${GIT_HASH}\")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/../include)
include_directories(${CMAKE_SOURCE_DIR}/blizzard)
include_directories(${CMAKE_SOURCE_DIR}/objects)

file(MAKE_DIRECTORY "libs")

set(SOURCE_EXE 
    ../include/serialization.cpp
    ../include/config_base.cpp
    ../include/http_server.cpp
    ../include/http_proto.cpp
    ../include/stat_base.cpp
    ../include/get_html.cpp
    ../include/response.cpp
    ../include/request.cpp
    ../include/crypto.cpp
    ../include/format.cpp
    ../include/stream.cpp
    ../include/config.cpp
    ../include/socket.cpp
    ../include/types.cpp
    ../include/admin.cpp
    ../include/util.cpp
    ../include/log.cpp
    ./objects/session.cpp
    ./objects/archive.cpp
    ./objects/message.cpp
    ./objects/invite.cpp
    ./objects/writer.cpp
    ./objects/image.cpp
    ./objects/video.cpp
    ./objects/audio.cpp
    ./objects/room.cpp
    ./blizzard/blizzard.cpp
    ./blizzard/table.cpp
    ./blizzard/object.cpp
    service_messages.cpp
    stat_daemon.cpp
    admin_chatd.cpp
    audio_tags.cpp
    service.cpp
    answer.cpp
#    dumper.cpp
    domain.cpp
    avatar.cpp
    smile.cpp
    front.cpp
    error.cpp
)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost COMPONENTS system regex thread date_time filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# Crypto++ =======================================
if (NOT EXISTS "${CMAKE_SOURCE_DIR}/libs/cryptopp")
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" clone https://github.com/weidai11/cryptopp.git
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/libs"
    )
endif()
add_subdirectory(libs/cryptopp)
include_directories(${CMAKE_SOURCE_DIR}/libs/cryptopp)
#=================================================


# Taglib =======================================
if (NOT EXISTS "${CMAKE_SOURCE_DIR}/libs/taglib")
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" clone https://github.com/taglib/taglib.git
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/libs"
    )
endif()
add_subdirectory(libs/taglib)
include_directories(${CMAKE_SOURCE_DIR}/libs/taglib)
include_directories(${CMAKE_SOURCE_DIR}/libs/taglib/taglib)
include_directories(${CMAKE_SOURCE_DIR}/libs/taglib/taglib/toolkit)
#=================================================

link_directories(${Boost_LIBRARY_DIRS} /usr/local/lib)

#message(STATUS "*****")

# Google test =======================================
if (NOT EXISTS "${CMAKE_SOURCE_DIR}/libs/googletest")
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" clone https://github.com/google/googletest.git
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/libs"
    )
endif()
add_subdirectory(libs/googletest)
include_directories(${CMAKE_SOURCE_DIR}/libs/googletest/googletest/include)
#=================================================

# RapidJSON =======================================
if (NOT EXISTS "${CMAKE_SOURCE_DIR}/libs/rapidjson")
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" clone https://github.com/miloyip/rapidjson.git
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/libs"
    )
endif()
add_subdirectory(libs/rapidjson)
include_directories(${CMAKE_SOURCE_DIR}/libs/rapidjson/include)
#=================================================

# WebSocket++ =======================================
#if (NOT EXISTS "${CMAKE_SOURCE_DIR}/libs/websocketpp")
#    execute_process(
#        COMMAND "${GIT_EXECUTABLE}" clone https://github.com/zaphoyd/websocketpp.git
#        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/libs"
#    )
#endif()
#add_subdirectory(libs/websocketpp)
#include_directories(${CMAKE_SOURCE_DIR}/libs/websocketpp)
#=================================================

add_executable(${PROJECT_NAME} ${SOURCE_EXE} chatd.cpp)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} cryptopp-static tag z)

add_executable(tests ${SOURCE_EXE} unit_tests.cpp)
target_link_libraries(tests ${Boost_LIBRARIES} cryptopp-static tag z gtest)

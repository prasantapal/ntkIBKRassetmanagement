
cmake_minimum_required(VERSION 4.2)
project(NTK)
set(HOME "$ENV{HOME}")
set(PROTOBUF_PATH "softwares/lib/protobuf/src")
message(${HOME})
include_directories(./  ./include /opt/homebrew/include ./client ./client/protobufUnix  ${HOME}/${PROTOBUF_PATH})




#file(GLOB SOURCES "*.cpp" "./protobufUnix/*.cc")
#message(${SOURCES})
#file(GLOB HEADERS "*.h")
#set(twsapi_VERSION 9.79.02)
##add_library(twsapi SHARED ${SOURCES} )
#include_directories(twsapi /usr/local/include ./protobufUnix)
#add_library(twsapi STATIC ${SOURCES} )
#

file(GLOB SOURCES "*.cpp")
# file(GLOB SOURCES "*.cpp" "./client/protobufUnix/*.cc" )
add_executable(TestCppClient ${SOURCES})
set_target_properties(TestCppClient
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
target_link_libraries(TestCppClient PRIVATE /usr/local/lib/libtwsapi.a  /usr/local/lib/libprotobuf.dylib /usr/local/lib/libbid.a )
#target_link_libraries(TestCppClient PRIVATE /usr/local/lib/libtwsapi.a  /usr/local/lib/libprotobuf.dylib /usr/local/lib/libbid.a )
# target_include_directories(TestCppClient PRIVATE  /opt/homebrew/include "${PROJECT_SOURCE_DIR}/source/cppclient/" "${PROJECT_SOURCE_DIR}/source/cppclient/client/")
#target_link_libraries(TestCppClient PUBLIC Threads::Threads)

if( MSVC )
  target_compile_options( TestCppClient PRIVATE /W3 )
else()
  target_compile_options( TestCppClient PRIVATE -std=c++2a -Wall -Wextra -pedantic -Wno-unused-parameter )
endif()

install(TARGETS TestCppClient DESTINATION bin)

FILE(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION "include/tws-api")

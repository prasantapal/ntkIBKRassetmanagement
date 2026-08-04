
cmake_minimum_required(VERSION 4.2)
project(NTK)
set(HOME "$ENV{HOME}")
set(PROTOBUF_PATH "softwares/lib/protobuf/src")
message("Home" ${HOME})
set (CC gcc-16)
set (CXX g++-16)

set(CMAKE_CXX_FLAGS " -Wuninitialized -Wno-psabi -O3 -fsigned-char  -ansi -Wall -Wextra -Weffc++ -fopenmp -fconcepts")


#set whether or not OpenCV is included
SET(CMAKE_C_COMPILER ${CC})  #set this as per your compiler gcc setting
SET(CMAKE_CXX_COMPILER ${CXX}) #set this as per your compiler gcc setting
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
set(CMAKE_VERBOSE_MAKEFILE ON) #Lets be a bit verbose to see everything that is happening
set(CMAKE_CXX_STANDARD 26) #set the standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CURRENT_PCL_VERSION 1.12)
set (CMAKE_BUILD_TYPE "Release")



include_directories(./  ./include ./include/fifo /opt/homebrew/include ./client ./client/protobufUnix  ${HOME}/${PROTOBUF_PATH})




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
# target_link_libraries(TestCppClient PRIVATE /usr/local/lib/libtwsapi_backup.a /usr/local/lib/libprotobuf.dylib /usr/local/lib/libbid.a )
# target_link_libraries(TestCppClient PRIVATE /usr/local/lib/libtwsapi.a  /usr/local/lib/libprotobuf.dylib /usr/local/lib/libbid.a )
 target_link_libraries(TestCppClient PRIVATE /usr/local/lib/libtwsapi.a  /usr/local/lib/libprotobuf.dylib /usr/local/lib/libbid.a )
# target_include_directories(TestCppClient PRIVATE  /opt/homebrew/include "${PROJECT_SOURCE_DIR}/source/cppclient/" "${PROJECT_SOURCE_DIR}/source/cppclient/client/")
#target_link_libraries(TestCppClient PUBLIC Threads::Threads)

if( MSVC )
  target_compile_options( TestCppClient PRIVATE /W3 )
else()
  target_compile_options( TestCppClient PRIVATE -Wall -Wextra -pedantic -Wno-unused-parameter )
endif()

install(TARGETS TestCppClient DESTINATION bin)

FILE(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION "include/tws-api")

cmake_minimum_required(VERSION 4.2)
project(twsapi LANGUAGES CXX)
file(GLOB SOURCES "*.cpp" "./protobufUnix/*.cc")
message(${SOURCES})
file(GLOB HEADERS "*.h")
set(twsapi_VERSION 9.79.02)

set(CMAKE_CXX_FLAGS "  -Wuninitialized -Wno-psabi -O3 -fsigned-char   -ansi -Wall -Wextra -Weffc++ -fopenmp -fconcepts")
#set(CMAKE_CXX_FLAGS "-isystem -mmacosx-version-min=10.0 -Wuninitialized -Wno-psabi -fsigned-char   -O2 -ansi -Wall -Wextra -Weffc++ -fopenmp -fconcepts -lpthread  -lgomp")
##########################################################################################################
##########################################################################################################
##########################################################################################################
message("Home" ${HOME})
#set whether or not OpenCV is included
SET(CMAKE_C_COMPILER ${CC})  #set this as per your compiler gcc setting
SET(CMAKE_CXX_COMPILER ${CXX}) #set this as per your compiler gcc setting
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
set(CMAKE_VERBOSE_MAKEFILE ON) #Lets be a bit verbose to see everything that is happening
set(CMAKE_CXX_STANDARD 23) #set the standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CURRENT_PCL_VERSION 1.12)
set (CMAKE_BUILD_TYPE "Release")
Find_package(PkgConfig REQUIRED)


#add_library(twsapi SHARED ${SOURCES} )
include_directories(twsapi /usr/local/include ./protobufUnix)
add_library(twsapi STATIC ${SOURCES} /usr/local/lib/libbid.a )
# On Windows (including MinGW), link against Winsock 2
#target_compile_options( twsapi PRIVATE -Wall -Wextra -pedantic -Wno-unused-parameter /usr/local/lib/libbid.a )
#/usr/local/lib/libbid.a /usr/local/lib/libprotobuf.dylib /usr/local/lib/libprotoc.dylib 
#set_target_properties(twsapi
#    PROPERTIES
#    IMPORTED_LOCATION /usr/local/lib/libbid.a 
#    IMPORTED_LOCATION /usr/local/lib/libprotobuf.dylib
#    PREFIX "lib"
#    DEBUG_POSTFIX "d"
#    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
#    PUBLIC_HEADER "${HEADERS}"
#)
 install(TARGETS twsapi EXPORT twsapi-targets PUBLIC_HEADER DESTINATION include/twsapi LIBRARY DESTINATION lib)

cmake_minimum_required(VERSION 4.2)
project(twsapi LANGUAGES CXX)
file(GLOB SOURCES "*.cpp" "./protobufUnix/*.cc")
message(${SOURCES})
file(GLOB HEADERS "*.h")
set(twsapi_VERSION 9.79.02)
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

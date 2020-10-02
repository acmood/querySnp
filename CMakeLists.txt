cmake_minimum_required(VERSION 3.17)
project(snp)

message(${PROJECT_NAME})
FILE(GLOB HEADER_FILES "src/*.hpp" "src/*.h")
FILE(GLOB SOURCE_FILES "src/*.cpp")
set(CMAKE_CXX_STANDARD 14)
 
link_directories(lib)
INCLUDE_DIRECTORIES(
        hiredis 
        adapters)

add_executable(${PROJECT_NAME} src/querySnp.cpp ${HEADER_FILES} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} libhiredis.a)

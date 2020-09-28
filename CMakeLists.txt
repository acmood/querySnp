cmake_minimum_required(VERSION 3.17)
project(snp)

message(${PROJECT_NAME})
FILE(GLOB HEADER_FILES "src/*.hpp" "src/*.h")
FILE(GLOB SOURCE_FILES "src/*.cpp")
set(CMAKE_CXX_STANDARD 14)
 
#ADD_LIBRARY(libhiredis STATIC /usr/local/lib/libhiredis.a) 
INCLUDE_DIRECTORIES(
        /usr/local/include/hiredis 
        /usr/local/include/hiredis/adapters)

#add_subdirectory(smhasher/src)

#target_link_libraries(${PROJECT_NAME} PRIVATE SMHasher)
#target_include_directories(${PROJECT_NAME} PRIVATE SMHasher)
link_directories(lib)
add_executable(${PROJECT_NAME} src/querySnp.cpp ${HEADER_FILES} ${SOURCE_FILES})
#TARGET_LINK_LIBRARIES(${PROJECT_NAME} libhiredis)
#SET_TARGET_PROPERTIES(libhiredis PROPERTIES LINKER_LANGUAGE C)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} libhiredis.a)

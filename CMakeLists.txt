CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(DbDeploy++)

#SET(CMAKE_BUILD_TYPE "debug")
#SET(CMAKE_BUILD_TYPE "release")

# ADD_DEFINITIONS(-fno-inline -Wall -O)

SET(CMAKE_USE_RELATIVE_PATHS "TRUE" CACHE BOOL "ON")

SET(RUNTIME_OUTPUT_DIRECTORY "${DbDeploy++_BINARY_DIR}/bin" CACHE INTERNAL "")
SET(LIBRARY_OUTPUT_DIRECTORY "${DbDeploy++_BINARY_DIR}/bin" CACHE INTERNAL "")

SET(EXECUTABLE_OUTPUT_PATH "${DbDeploy++_BINARY_DIR}/bin" CACHE INTERNAL "")
SET(LIBRARY_OUTPUT_PATH "${DbDeploy++_BINARY_DIR}/bin" CACHE INTERNAL "")

# -DMODDED -DNOPCH 
ADD_DEFINITIONS(-D_UNICODE -DUNICODE -D_LIB)

INCLUDE_DIRECTORIES(include 
	/usr/local/boost_1_44_0
	/opt/local/include/postgresql90/
)
LINK_DIRECTORIES(
	/opt/local/lib/postgresql90 
)
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

ENABLE_TESTING()

add_subdirectory(unittest-cpp)
add_subdirectory(test)
add_subdirectory(core)
add_subdirectory(cli)
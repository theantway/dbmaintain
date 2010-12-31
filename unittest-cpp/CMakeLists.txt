PROJECT(unitTest++)
REMOVE_DEFINITIONS(-DUNICODE)

INCLUDE_DIRECTORIES(
	UnitTest++/src
	UnitTest++/src/Posix
)

FILE(GLOB inFiles RELATIVE 
			${CMAKE_CURRENT_SOURCE_DIR}
			${CMAKE_CURRENT_SOURCE_DIR}/UnitTest++/src/*.h
			${CMAKE_CURRENT_SOURCE_DIR}/UnitTest++/src/*.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/UnitTest++/src/Posix/*.h
			${CMAKE_CURRENT_SOURCE_DIR}/UnitTest++/src/Posix/*.cpp
)
			
ADD_LIBRARY(unitTest++ ${inFiles})
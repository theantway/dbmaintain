PROJECT(baseTest)

FILE(GLOB inFiles RELATIVE 
			"${CMAKE_CURRENT_SOURCE_DIR}"
			"${CMAKE_CURRENT_SOURCE_DIR}/src/*" 
			"${CMAKE_CURRENT_SOURCE_DIR}/include/*")

INCLUDE_DIRECTORIES(include 
	${unitTest++_SOURCE_DIR}/UnitTest++/src
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

ADD_LIBRARY(baseTest ${inFiles})

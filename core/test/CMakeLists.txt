PROJECT(coreTest)

# Find all the input files
FILE(GLOB_RECURSE inFiles RELATIVE
    ${CMAKE_CURRENT_SOURCE_DIR}
	*.cpp
	${baseTest_SOURCE_DIR}/Main.cpp
)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${baseTest_SOURCE_DIR}/include
	${unitTest++_SOURCE_DIR}/UnitTest++/src
	${core_SOURCE_DIR}/include
)

ADD_EXECUTABLE(coreTests ${inFiles})

add_dependencies(coreTests
    baseTest
	unitTest++
	dbdeploycore
)

TARGET_LINK_LIBRARIES(coreTests
	baseTest
	unitTest++
	dbdeploycore
	pq
)

ADD_TEST(coreTest ${EXECUTABLE_OUTPUT_PATH}/coreTests)
PROJECT(core)

FILE(GLOB inFiles RELATIVE 
    ${CMAKE_CURRENT_SOURCE_DIR}
	src/*
	include/*
)

INCLUDE_DIRECTORIES(include 
	../core/include
)

SET(BINARY ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_CFG_INTDIR})

ADD_LIBRARY(dbdeploycore ${inFiles})

TARGET_LINK_LIBRARIES(dbdeploycore
	pq
	${OPENSSL_LIBRARIES}
)

add_subdirectory(test)
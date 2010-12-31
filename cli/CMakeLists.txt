PROJECT(cli)

FILE(GLOB inFiles RELATIVE 
    ${CMAKE_CURRENT_SOURCE_DIR}
	src/*
	include/*
)

INCLUDE_DIRECTORIES(include 
	${core_SOURCE_DIR}/include
)

LINK_DIRECTORIES (
	${wxWidgets_LIB_DIR}
)

SET(BINARY ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_CFG_INTDIR})

ADD_EXECUTABLE(DBDeploy ${inFiles})

TARGET_LINK_LIBRARIES(DBDeploy
    dbdeploycore
)

add_subdirectory(CPU)
add_subdirectory(assets)

add_subdirectory(debugger)

set(SOURCES
	${SOURCES}
	${CMAKE_CURRENT_SOURCE_DIR}/zenith80.cpp
	PARENT_SCOPE
)

set(DEBUGGER_SOURCES
	${DEBUGGER_SOURCES}
	PARENT_SCOPE
)

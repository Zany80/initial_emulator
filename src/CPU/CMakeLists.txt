add_subdirectory(peripherals)

set(SOURCES
	${SOURCES}
	${CMAKE_CURRENT_SOURCE_DIR}/Z80.cpp
	PARENT_SCOPE
)

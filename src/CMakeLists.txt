add_subdirectory(CPU)
add_subdirectory(assets)

set(SOURCES
	${SOURCES}
	${CMAKE_CURRENT_SOURCE_DIR}/zenith80.cpp
	PARENT_SCOPE
)

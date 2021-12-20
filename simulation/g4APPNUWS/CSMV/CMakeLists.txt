file(GLOB CSMV_SOURCES "src/*.cc")
include_directories(${PROJECT_SOURCE_DIR}/PXSTbs/include
		            ${PROJECT_SOURCE_DIR}/CSMV/include)
add_library(CSMV SHARED ${CSMV_SOURCES})
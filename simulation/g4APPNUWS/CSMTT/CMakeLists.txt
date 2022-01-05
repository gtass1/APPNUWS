file(GLOB CSMTT_SOURCES "src/*.cc")
include_directories(${PROJECT_SOURCE_DIR}/PXSTbs/include
		            ${PROJECT_SOURCE_DIR}/CSMTT/include)
add_library(CSMTT SHARED ${CSMTT_SOURCES})
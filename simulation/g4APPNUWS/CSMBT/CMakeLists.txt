file(GLOB CSMBT_SOURCES "src/*.cc")
include_directories(${PROJECT_SOURCE_DIR}/PXSTbs/include
		            ${PROJECT_SOURCE_DIR}/CSMBT/include)
add_library(CSMBT SHARED ${CSMBT_SOURCES})
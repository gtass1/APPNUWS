file(GLOB PXSTBS_SOURCES "src/*.cc")
include_directories(${PROJECT_SOURCE_DIR}/PXSTbs/include)
add_library(PXSTbs SHARED ${PXSTBS_SOURCES})

file(GLOB DCHBS_SOURCES "src/*.cc")
include_directories(${PROJECT_SOURCE_DIR}/DCHbs/include)
add_library(DCHbs SHARED ${DCHBS_SOURCES})

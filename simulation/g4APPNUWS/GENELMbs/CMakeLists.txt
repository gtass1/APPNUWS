file(GLOB GENELMBS_SOURCES "src/*.cc")
include_directories(${PROJECT_SOURCE_DIR}/GENELMbs/include)
add_library(GENELMbs SHARED ${GENELMBS_SOURCES})

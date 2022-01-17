file(GLOB FLOOR_SOURCES "src/*.cc")
include_directories(${PROJECT_SOURCE_DIR}/GENELMbs/include
                    ${PROJECT_SOURCE_DIR}/FLOOR/include)
add_library(FLOOR SHARED ${FLOOR_SOURCES})
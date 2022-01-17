file(GLOB TRUCK_SOURCES "src/*.cc")
include_directories(${PROJECT_SOURCE_DIR}/GENELMbs/include
                    ${PROJECT_SOURCE_DIR}/TRUCK/include)
add_library(TRUCK SHARED ${TRUCK_SOURCES})
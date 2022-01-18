file(GLOB CARGO_SOURCES "src/*.cc")
include_directories(${PROJECT_SOURCE_DIR}/GENELMbs/include
                    ${PROJECT_SOURCE_DIR}/TRUCK/include
                    ${PROJECT_SOURCE_DIR}/CARGO/include)
add_library(CARGO SHARED ${CARGO_SOURCES})
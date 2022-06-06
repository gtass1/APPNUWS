
#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(g4APPNUWS_simulation)

set(BASE_FLD
	EXCEP
	IOSRVC
	GEOMSRVC
)

set(DETBS_FLD
	GENELMbs
	PXSTbs
	DCHbs
)

set(DET_FLD
	CSMTT
	CSMBT
	TRUCK
	CARGO
	FLOOR
)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
#include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
find_package(Boost REQUIRED)
#include_directories(${BOOST_ROOT}/include)
include_directories(${Boost_INCLUDE_DIRS})

#----------------------------------------------------------------------------
# Find HepMC (required package)
#
find_package(HepMC REQUIRED)
#find_package(EDM4HEP)
#find_package(podio)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS RIO EG)
include(${ROOT_USE_FILE})
include_directories(${ROOT_INCLUDE_DIRS})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
include_directories(${PROJECT_SOURCE_DIR}/include
                    ${Geant4_INCLUDE_DIR}
                    ${HEPMC_INCLUDE_DIR}
                    ${PROJECT_SOURCE_DIR}/../ConfigReader/include
		    )
foreach(basefld ${BASE_FLD})
	include_directories(${PROJECT_SOURCE_DIR}/${basefld}/include)
endforeach()
foreach(detbsfld ${DETBS_FLD})
	include_directories(${PROJECT_SOURCE_DIR}/${detbsfld}/include)
endforeach()


file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

#------------------
# dictionary
ROOT_GENERATE_DICTIONARY(ClassesDict SimG4Classes.hh LINKDEF ${PROJECT_SOURCE_DIR}/IOSRVC/src/LinkDef.hh)
add_library(SimG4ClassesDict SHARED
	${PROJECT_SOURCE_DIR}/IOSRVC/src/SimG4TrackerHit.cc
	${PROJECT_SOURCE_DIR}/IOSRVC/src/SimG4Particle.cc
	        ${PROJECT_SOURCE_DIR}/IOSRVC/include/SimG4Classes.hh
	        ClassesDict.cxx
			)

add_custom_command(TARGET SimG4ClassesDict POST_BUILD
		    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/libClassesDict* ${PROJECT_BINARY_DIR}/lib
            COMMAND ${CMAKE_COMMAND} -E remove ${PROJECT_BINARY_DIR}/libClassesDict*
		    )

#------------------
# add subdirectories

add_subdirectory(../ConfigReader ${CMAKE_CURRENT_BINARY_DIR}/ConfigReader)
foreach(basefld ${BASE_FLD})
	add_subdirectory(${basefld})
endforeach()
foreach(detbsfld ${DETBS_FLD})
	add_subdirectory(${detbsfld})
endforeach()
foreach(detfld ${DET_FLD})
	add_subdirectory(${detfld})
endforeach()

#----------------------------------------------------------------------------
# Make progect shared libraries
#

add_library(G4APPNUWS SHARED ${sources})
foreach(detfld ${DET_FLD})
	target_include_directories(G4APPNUWS
		PUBLIC ${PROJECT_SOURCE_DIR}/${detfld}/include
	)
endforeach()

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#

add_executable(g4APPNUWS g4APPNUWS.cc)

target_link_libraries(g4APPNUWS PUBLIC
		      G4APPNUWS
		      SimG4ClassesDict
		      SCrd
			  ${BASE_FLD}
			  ${DETBS_FLD}
			  ${DET_FLD}
		      ${Geant4_LIBRARIES}
              ${HEPMC_LIBRARIES}
		      ${HEPMC_FIO_LIBRARIES}
              ${PYTHIA6_LIBRARIES}
		      ${ROOT_LIBRARIES}
#		      ${${PROJECT_NAME}_LIB}
#		      gfortran
#		      EDM4HEP::edm4hep
#		      podio::podioRootIO
		      )

#----------------------------------------------------------------------------
# Link configure files

add_custom_command(TARGET g4APPNUWS POST_BUILD
		   COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/config ${PROJECT_BINARY_DIR}/config
           COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/g4mac ${PROJECT_BINARY_DIR}/g4mac
		   )

#----------------------------------------------------------------------------
# Copy local headers for this project
#
# file(COPY
	# ${PROJECT_SOURCE_DIR}/../ConfigReader/include/
    # DESTINATION include/ConfigReader
# )
add_custom_command(
#	linkConfigReader ALL
TARGET g4APPNUWS POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/include
	COMMAND ${CMAKE_COMMAND} -E create_symlink ${PROJECT_SOURCE_DIR}/../ConfigReader/include ${PROJECT_BINARY_DIR}/include/ConfigReader
)
foreach(basefld ${BASE_FLD})
	# file(COPY
		# ${PROJECT_SOURCE_DIR}/${basefld}/include/
		# DESTINATION include/${basefld}
	# )
	add_custom_command(
#		OUTPUT link${basefld}
TARGET g4APPNUWS POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E create_symlink ${PROJECT_SOURCE_DIR}/${basefld}/include ${PROJECT_BINARY_DIR}/include/${basefld}
	)
endforeach()
foreach(detbsfld ${DETBS_FLD})
	# file(COPY
		# ${PROJECT_SOURCE_DIR}/${detbsfld}/include/
		# DESTINATION include/${detbsfld}
	# )
	add_custom_command(
		TARGET g4APPNUWS POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E create_symlink ${PROJECT_SOURCE_DIR}/${detbsfld}/include ${PROJECT_BINARY_DIR}/include/${detbsfld}
	)
endforeach()
foreach(detfld ${DET_FLD})
	# file(COPY
		# ${PROJECT_SOURCE_DIR}/${detfld}/include/
		# DESTINATION include/${detfld}
	# )
	add_custom_command(
		TARGET g4APPNUWS POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E create_symlink ${PROJECT_SOURCE_DIR}/${detfld}/include ${PROJECT_BINARY_DIR}/include/${detfld}
	)
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS g4APPNUWS DESTINATION bin)

install(DIRECTORY ${PROJECT_BINARY_DIR}/lib DESTINATION .)

install(DIRECTORY ../ConfigReader/include/ DESTINATION include/ConfigReader)
foreach(basefld ${BASE_FLD})
	install(DIRECTORY ${basefld}/include/ DESTINATION include/${basefld})
endforeach()
foreach(detbsfld ${DETBS_FLD})
	install(DIRECTORY ${detbsfld}/include/ DESTINATION include/${detbsfld})
endforeach()
foreach(detfld ${DET_FLD})
	install(DIRECTORY ${detfld}/include/ DESTINATION include/${detfld})
endforeach()

install(DIRECTORY g4mac DESTINATION .)
install(DIRECTORY config DESTINATION .)


#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(g4APPNUWS_converter)

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

#----------------------------------------------------------------------------
# Find HepMC (required package)

find_package(HepMC REQUIRED)

#----------------------------------------------------------------------------
# Find EDM/podio
#
#find_package(EDM4HEP)
#find_package(podio)
#find_package(DD4hep)

#----------------------------------------------------------------------------
# Find ROOT
#
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS RIO EG)
include(${ROOT_USE_FILE})
include_directories(${ROOT_INCLUDE_DIRS})

#----------------------------------------------------------------------------
# THIS PROJECT
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#--------------------------------------------------------------------
# headers
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
include_directories($ENV{G4WORKDIR}/include
        $ENV{PRJBASE}/analyzer/rAPPNUWS/include
#        ${Geant4_INCLUDE_DIR}
#        ${HEPMC_INCLUDE_DIR}
#        $ENV{PRJBASE}/simulation/ConfigReader/include
        $ENV{ROOTSYS}/include
        $ENV{G4INCLUDE}/Geant4
        $ENV{ROMESYS}/include/
#        $ENV{PRJBASE}/simulation/g4APPNUWS
#        $ENV{CLHEP_INCLUDE_DIR}
)

#foreach(basefld ${BASE_FLD})
#include_directories($ENV{PRJBASE}/simulation/${basefld}/include)
#endforeach()

# -------------------------------------
add_executable(readHits readHits.cc)

target_link_libraries(readHits PUBLIC
        G4APPNUWS
        SimG4ClassesDict
        rAPPNUWSAnalyzer
        stdc++
        SCrd
        ${BASE_FLD}
        ${DETBS_FLD}
        ${DET_FLD}
        ${Geant4_LIBRARIES}
        ${HEPMC_LIBRARIES}
        ${HEPMC_FIO_LIBRARIES}
#        ${PYTHIA6_LIBRARIES}
        ${ROOT_LIBRARIES}
#        Core
#        Imt
#        RIO
#        Geom
#        Net
#        Hist
#        Graf
#        Graf3d
#        Gpad
#        ROOTVecOps
#        Tree
#        TreePlayer
#        Rint
#        Postscript
#        Matrix
#        Physics
#        Minuit
        Html
#        Gui
#        MathCore
#        Thread
#        MultiProc
#        ROOTDataFrame
#        pthread
#        GL
#        GLU
#        Xrandr
#        Xext
#        X11
#        EDM4HEP::edm4hep
#        podio::podioRootIO
        )

target_link_directories(readHits PUBLIC
#		$ENV{CLHEP_LIB_DIR}
		$ENV{G4LIB}
#		$ENV{SIM_INSTAL_DIR}/lib
        $ENV{G4WORKDIR}/lib
		$ENV{PRJBASE}/analyzer/rAPPNUWS/obj
#		$ENV{GENFIT2SYS}/lib
)	      

#--------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
#install(TARGETS g4APPNUWS DESTINATION bin)
#install(DIRECTORY ${PROJECT_BINARY_DIR}/lib DESTINATION .)

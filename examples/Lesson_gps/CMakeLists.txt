CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

PROJECT(lesson03)

set(Geant4_DIR /home/zelenyy/GEANT4/install/lib/Geant4-10.3.0/)

FIND_PACKAGE(Geant4 REQUIRED ui_all vis_all)

IF(NOT Geant4_FOUND)
    MESSAGE(FATAL_ERROR "Geant4 not found!")
ENDIF(NOT Geant4_FOUND)

INCLUDE(${Geant4_USE_FILE})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

FILE(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)


ADD_EXECUTABLE(lesson03.exe lesson03.cc ${sources})
TARGET_LINK_LIBRARIES(lesson03.exe ${Geant4_LIBRARIES})


# Copy all scripts to the build directory
set( GEANT4_SCRIPTS init_vis.mac)

foreach(_script ${GEANT4_SCRIPTS})
    configure_file(
            ${PROJECT_SOURCE_DIR}/${_script}
            ${PROJECT_BINARY_DIR}/${_script}
            COPYONLY
    )
endforeach()

install(TARGETS lesson03.exe DESTINATION bin)





CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

PROJECT(geant4-emcalculator)

#set(Geant4_DIR  /home/zelenyy/GEANT4/install/lib/Geant4-10.3.0/)

FIND_PACKAGE(Geant4 REQUIRED ui_all vis_all)

IF(NOT Geant4_FOUND)
    MESSAGE(FATAL_ERROR "Geant4 not found!")
ENDIF(NOT Geant4_FOUND)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
INCLUDE(${Geant4_USE_FILE})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../g4npm/include)

find_library(geant4 NAMES geant4
        PATHS ${PROJECT_SOURCE_DIR}/../g4npm/lib)
find_library(utils NAMES utils
        PATHS ${PROJECT_SOURCE_DIR}/../g4npm/lib)

FILE(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

ADD_EXECUTABLE(geant4-emcalculator.exe main.cc ${sources} ${headers})

TARGET_LINK_LIBRARIES(geant4-emcalculator.exe ${Geant4_LIBRARIES} ${geant4} ${utils})







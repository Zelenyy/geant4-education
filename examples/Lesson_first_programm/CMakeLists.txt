PROJECT(geant4-thunderstorm)

#set(Geant4_DIR  /home/zelenyy/GEANT4/install/lib/Geant4-10.3.0/)

FIND_PACKAGE(Geant4 REQUIRED ui_all vis_all)

IF(NOT Geant4_FOUND)
    MESSAGE(FATAL_ERROR "Geant4 not found!")
ENDIF(NOT Geant4_FOUND)

INCLUDE(${Geant4_USE_FILE})
ADD_EXECUTABLE(lesson01.exe lesson01.cc)
TARGET_LINK_LIBRARIES(lesson01.exe ${Geant4_LIBRARIES})

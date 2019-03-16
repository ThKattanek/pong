# CMakeList.txt for PONG by Thorsten Kattanek
# Berlin, 16-03-2019
#
# For Static Win32 compilation must -DSFML_STATIC_LIBRARIES=TRUE
# Installprefix set with: -DCMAKE_INSTALL_PREFIX=[INSTALL_PATH]
#
################################################################

cmake_minimum_required(VERSION 2.8)

##################### PROJECT #####################

project("pong")
message(STATUS "Project Name: " ${PROJECT_NAME})

#################### CONFIGURE ####################

# Standard Build Type = Release
if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
        message(STATUS "CMAKE_BUILD_TYPE NOT FOUND")
endif()

if(WIN32)
    if(WIN32_STATIC_BUILD)
        add_definitions(-DSFML_STATIC)
        set(SFML_STATIC_LIBRARIES TRUE)
    endif()
endif()

if(WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s -Os -g -Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s -Os -g -Wall")
endif()

# Data Directory
set(DATA_DIRECTORY_NAME "data")

##################### SOURCES #####################

set(SOURCES
    src/main.cpp
)

################### RESOURCEN #####################

if(WIN32)
    set(WIN_RC ${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}.rc)
endif()

################### LIBRARIES #####################

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# SFML
# network audio graphics window system
find_package(SFML 2 COMPONENTS system window graphics audio REQUIRED)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    set(LIBRARIES ${LIBRARIES} ${SFML_SYSTEM_LIBRARY})
    set(LIBRARIES ${LIBRARIES} ${SFML_SYSTEM_DEPENDENCIES})
    set(LIBRARIES ${LIBRARIES} ${SFML_WINDOW_LIBRARY})
    set(LIBRARIES ${LIBRARIES} ${SFML_WINDOW_DEPENDENCIES})
    set(LIBRARIES ${LIBRARIES} ${SFML_GRAPHICS_LIBRARY})
    set(LIBRARIES ${LIBRARIES} ${SFML_GRAPHICS_DEPENDENCIES})
    set(LIBRARIES ${LIBRARIES} ${SFML_AUDIO_LIBRARY})
    set(LIBRARIES ${LIBRARIES} ${SFML_AUDIO_DEPENDENCIES})
    if(WIN32)
        if(WIN32_STATIC_BUILD)
            #message(STATUS ${WIN32_LIBS})
            #set(LIBRARIES ${LIBRARIES} -mwindows -lsfml-system-s png jpeg harfbuzz z bz2 winmm)
            set(LIBRARIES ${LIBRARIES} ${WIN32_LIBS})
        endif()
    endif()
else()
    message(STATUS "SFML not found!")
endif()

################### EXECUTABLE ####################

if(NOT WIN32)
    add_executable(${PROJECT_NAME} ${SOURCES})
else()
    add_executable(${PROJECT_NAME} ${SOURCES} ${WIN_RC})
endif()

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

################### INSTALL #######################

if(WIN32)
# Windows

    # Set DATA_PATH definition
    add_definitions("-DDATA_PATH=\"${DATA_DIRECTORY_NAME}\"")

    # Executable
    install(TARGETS ${PROJECT_NAME} DESTINATION ./)

    # Files
    install(FILES "data/num.png" DESTINATION "./${DATA_DIRECTORY_NAME}")
    install(FILES "data/sound1.ogg" DESTINATION "./${DATA_DIRECTORY_NAME}")
    install(FILES "data/sound2.ogg" DESTINATION "./${DATA_DIRECTORY_NAME}")
    install(FILES "LICENSE" DESTINATION ./)
else()
# Linux

    # Set DATA_PATH definition
    add_definitions("-DDATA_PATH=\"${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}/${DATA_DIRECTORY_NAME}\"")

    # Executable
    install(TARGETS ${PROJECT_NAME} DESTINATION bin)

    # Files (nach share)
    install(FILES "data/num.png" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}/${DATA_DIRECTORY_NAME})
    install(FILES "data/sound1.ogg" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}/${DATA_DIRECTORY_NAME})
    install(FILES "data/sound2.ogg" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}/${DATA_DIRECTORY_NAME})
    install(FILES "LICENSE" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME})
endif()

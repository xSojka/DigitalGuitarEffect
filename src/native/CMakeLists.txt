file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

IF(WIN32)
    set(CMAKE_GENERATOR "Visual Studio 17 2022 Win64")
    message("Generator is set to ${CMAKE_GENERATOR}")
ENDIF()

add_library(${CMAKE_PROJECT_NAME}_lib SHARED ${SOURCES})

IF(WIN32)
    target_link_libraries(${CMAKE_PROJECT_NAME}_lib PUBLIC ${CMAKE_SOURCE_DIR}/lib/portaudio/portaudio.lib)
ELSEIF(APPLE)
    target_link_libraries(${CMAKE_PROJECT_NAME}_lib PUBLIC ${CMAKE_SOURCE_DIR}/lib/portaudio/build/libportaudio.dylib)
ELSE()
    target_link_libraries(${CMAKE_PROJECT_NAME}_lib PUBLIC ${CMAKE_SOURCE_DIR}/lib/portaudio/libportaudio.a rt m asound pthread)
ENDIF()


#add_library(${CMAKE_PROJECT_NAME}_lib_static STATIC ${SOURCES})

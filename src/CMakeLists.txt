set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

function(CUSTOM_RESOURCES outfiles )
  set(options)
  set(oneValueArgs)
  set(multiValueArgs OPTIONS DEPENDS)

  cmake_parse_arguments(_RCC "${options}" "${onValueArgs}" "${multiValueArgs}" ${ARGN})

  set(rcc_files ${_RCC_UNPARSED_ARGUMENTS})
  set(rcc_options ${_RCC_OPTIONS})

  if("${rcc_options}" MATCHES "-binary")
    message(WARNING "Use qt5_add_binary_resources for binary option")
  endif()

  foreach(it ${rcc_files})
    get_filename_component(outfilename ${it} NAME_WE)
    get_filename_component(infile ${it} ABSOLUTE)
    set(outfile ${CMAKE_CURRENT_BINARY_DIR}/qrc_${outfilename}.cpp)

    _QT5_PARSE_QRC_FILE(${infile} _out_depends _rc_depends)

    add_custom_command(OUTPUT ${outfile}
                      COMMAND ${Qt5Core_RCC_EXECUTABLE}
                      ARGS ${rcc_options} --name ${outfilename} --output ${outfile} ${infile}
                      MAIN_DEPENDENCY ${infile}
                      DEPENDS ${_rc_depends} "${out_depends}" ${_RCC_DEPENDS} VERBATIM)
    list(APPEND ${outfiles} ${outfile})
  endforeach()
  set(${outfiles} ${${outfiles}} PARENT_SCOPE)
endfunction()

set(ICONS_QRC ${CMAKE_BINARY_DIR}/icons.qrc)
configure_file(${CMAKE_SOURCE_DIR}/icons.yml.in ${CMAKE_BINARY_DIR}/icons.yml)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/icons.qrc
  COMMAND ${CMAKE_SOURCE_DIR}/material/scripts/icons.py ${CMAKE_BINARY_DIR}/icons.yml
  DEPENDS ${CMAKE_BINARY_DIR}/icons.yml)

set(liri_vinyl_SOURCES
  main.cpp
  song.cpp
  album.cpp
  albummodel.cpp
  artist.cpp
  musicdatabase.cpp
  musicscanner.cpp
  albumartprovider.cpp
  settings.cpp
  ${VENDOR_SOURCES}
)

set(liri_vinyl_HEADERS
    album.h
    albumartprovider.h
    albummodel.h
    artist.h
    database.h
    musicdatabase.h
    musicscanner.h
    song.h
    settings.h
    schema.h
)

custom_resources(liri_vinyl_RESOURCES 
  qml.qrc
  ${CMAKE_SOURCE_DIR}/material/src/material.qrc
  ${CMAKE_SOURCE_DIR}/material/src/components/components.qrc
  ${CMAKE_SOURCE_DIR}/material/src/controls/controls.qrc
  ${CMAKE_SOURCE_DIR}/material/src/core/core.qrc
  ${CMAKE_SOURCE_DIR}/material/src/extras/extras.qrc
  ${CMAKE_SOURCE_DIR}/material/src/listitems/listitems.qrc
  ${CMAKE_SOURCE_DIR}/material/src/popups/popups.qrc
  ${CMAKE_SOURCE_DIR}/material/src/styles/styles.qrc
  ${CMAKE_SOURCE_DIR}/material/src/window/window.qrc
  ${CMAKE_SOURCE_DIR}/material/icons/core_icons.qrc
  ${CMAKE_BINARY_DIR}/icons.qrc
  DEPENDS ${CMAKE_BINARY_DIR}/icons.qrc)

add_executable(${CMAKE_PROJECT_NAME} ${liri_vinyl_SOURCES} ${liri_vinyl_RESOURCES})
target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE
    cxx_noexcept
)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE -DBOOST_PP_VARIADICS)
file(GLOB_RECURSE QML_FILES "*.qml")
add_custom_target(qmlfiles SOURCES ${QML_FILES})
add_custom_target(headers SOURCES ${liri_vinyl_HEADERS})
target_link_libraries(${CMAKE_PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Svg Qt5::Qml Qt5::Quick Qt5::WebEngine Qt5::Sql ${TAGLIB_LIBRARIES} ${QTGSTREAMER_LIBRARIES})

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION bin)

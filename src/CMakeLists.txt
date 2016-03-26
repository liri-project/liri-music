set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(liri_vinyl_SOURCES
  main.cpp
  utilities.cpp
  songobject.cpp
  albumobject.cpp
  artistobject.cpp
  musicfolders.cpp
)

qt5_add_resources(liri_vinyl_RESOURCES qml.qrc)

add_executable(${CMAKE_PROJECT_NAME} ${liri_vinyl_SOURCES} ${liri_vinyl_RESOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Svg Qt5::Qml Qt5::Quick Qt5::Sql ${TAGLIB_LIBRARIES})


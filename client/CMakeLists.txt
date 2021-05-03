cmake_minimum_required(VERSION 3.9)
project(traintastic-client VERSION 0.0.1 DESCRIPTION "Traintastic client")
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(traintastic-client dark.qrc)

add_definitions(-DQT_DEPRECATED_WARNINGS)

target_include_directories(traintastic-client PRIVATE
  ../shared/src
  ../shared/thirdparty
  thirdparty)

file(GLOB SOURCES
  "src/*.hpp"
  "src/*.cpp"
  "src/board/*.hpp"
  "src/board/*.cpp"
  "src/dialog/*.hpp"
  "src/dialog/*.cpp"
  "src/network/*.hpp"
  "src/network/*.cpp"
  "src/style/*.hpp"
  "src/style/*.cpp"
  "src/subwindow/*.hpp"
  "src/subwindow/*.cpp"
  "src/utils/*.hpp"
  "src/utils/*.cpp"
  "src/widget/*.hpp"
  "src/widget/*.cpp"
  "src/widget/object/*.hpp"
  "src/widget/object/*.cpp"
  "../shared/src/traintastic/locale/locale.cpp"
  "thirdparty/QtWaitingSpinner/*.hpp"
  "thirdparty/QtWaitingSpinner/*.cpp")

find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

target_sources(traintastic-client PRIVATE ${SOURCES})

target_link_libraries(traintastic-client PRIVATE Qt5::Widgets Qt5::Network)

### INSTALL ###

install(TARGETS traintastic-client
    COMPONENT traintastic-client
    RUNTIME DESTINATION "/opt/traintastic/bin/"
)

### DEB package ###

if(NOT DEBIAN_PACKAGE_VERSION_EXTRA)
  set(DEBIAN_PACKAGE_VERSION_EXTRA "${PROJECT_VERSION_EXTRA}")
endif()

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}${DEBIAN_PACKAGE_VERSION_EXTRA}")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_SECTION "misc")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Reinder Feenstra")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://traintastic.org")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)
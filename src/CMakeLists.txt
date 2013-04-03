cmake_minimum_required(VERSION 2.6)

set(QWEBSOCKET_VERSION_MAJOR "1")
set(QWEBSOCKET_VERSION_MINOR "0")
set(QWEBSOCKET_VERSION_PATCH "0")
set(QWEBSOCKET_VERSION "${QWEBSOCKET_VERSION_MAJOR}.${QWEBSOCKET_VERSION_MINOR}")
set(QWEBSOCKET_VERSION_FULL "${QWEBSOCKET_VERSION}.${QWEBSOCKET_VERSION_PATCH}")

include(FindPkgConfig)

find_package(Qt4 REQUIRED)
find_package(X11 REQUIRED)
find_package(Boost 1.48 COMPONENTS system thread random regex REQUIRED)

set(WEBSOCKETPP_SRCDIR ${PROJECT_SOURCE_DIR}/3rdparty/websocketpp/src)

include(${QT_USE_FILE})
include_directories(${QT_QTCORE_INCLUDE_DIR}
  ${QT_QTGUI_INCLUDE_DIR}
  ${QT_QTXML_INCLUDE_DIR}
  ${QT_QTDBUS_INCLUDE_DIR}
  ${QT_QTNETWORK_INCLUDE_DIR}
  ${WEBSOCKETPP_SRCDIR}
  .)

set(WEBSOCKETPP_SOURCES
  ${WEBSOCKETPP_SRCDIR}/base64/base64.cpp
  ${WEBSOCKETPP_SRCDIR}/md5/md5.c
  ${WEBSOCKETPP_SRCDIR}/messages/data.cpp
  ${WEBSOCKETPP_SRCDIR}/network_utilities.cpp
  ${WEBSOCKETPP_SRCDIR}/processors/hybi_header.cpp
  ${WEBSOCKETPP_SRCDIR}/processors/hybi_util.cpp
  ${WEBSOCKETPP_SRCDIR}/sha1/sha1.cpp
  ${WEBSOCKETPP_SRCDIR}/uri.cpp)

set(SOURCES
  QWebSocket.cpp
  QWebSocketWrapper.cpp)

file(GLOB WEBSOCKETPP_SOURCES ${WEBSOCKETPP_SRCDIR}/*.cpp)

qt4_wrap_cpp(MOC_SOURCES
  QWebSocket.h
  QWebSocketWrapper.h)

add_library(qwebsocket SHARED
  ${WEBSOCKETPP_SOURCES}
  ${WEBSOCKETPP_SRCDIR}/base64/base64.cpp
  ${WEBSOCKETPP_SRCDIR}/sha1/sha1.cpp
  ${SOURCES}
  ${MOC_SOURCES})

set_target_properties(qwebsocket PROPERTIES VERSION ${QWEBSOCKET_VERSION_FULL} SOVERSION ${QWEBSOCKET_VERSION})

target_link_libraries(qwebsocket ${Boost_LIBRARIES})

file(GLOB HEADERS "*.h")
install_files(/include FILES ${HEADERS})

install(TARGETS qwebsocket DESTINATION lib)

if(UNIX)
  configure_file(../qwebsocket.pc.in qwebsocket.pc @ONLY)
  install(FILES qwebsocket.pc DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)
endif()

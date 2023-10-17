cmake_minimum_required(VERSION 3.9)

project(EasyMaster VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(EasyMaster
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        Icon_resource.qrc
        soem/inc/ethercat.h soem/inc/ethercatbase.h soem/inc/ethercatcoe.h soem/inc/ethercatconfig.h soem/inc/ethercatconfiglist.h soem/inc/ethercatdc.h soem/inc/ethercateoe.h soem/inc/ethercatfoe.h soem/inc/ethercatmain.h soem/inc/ethercatprint.h soem/inc/ethercatsoe.h soem/inc/ethercattype.h soem/inc/inttypes.h soem/inc/nicdrv.h soem/inc/osal.h soem/inc/osal_defs.h soem/inc/osal_win32.h soem/inc/oshw.h soem/inc/stdint.h soem/lib/soem.lib soem/wpcap/Include/pcap/bluetooth.h soem/wpcap/Include/pcap/bpf.h soem/wpcap/Include/pcap/namedb.h soem/wpcap/Include/pcap/pcap.h soem/wpcap/Include/pcap/sll.h soem/wpcap/Include/pcap/usb.h soem/wpcap/Include/pcap/vlan.h soem/wpcap/Include/bittypes.h soem/wpcap/Include/ip6_misc.h soem/wpcap/Include/Packet32.h soem/wpcap/Include/pcap-bpf.h soem/wpcap/Include/pcap-namedb.h soem/wpcap/Include/pcap-stdinc.h soem/wpcap/Include/pcap.h soem/wpcap/Include/remote-ext.h soem/wpcap/Include/Win32-Extensions.h soem/wpcap/Lib/x64/Packet.lib soem/wpcap/Lib/x64/wpcap.lib soem/wpcap/Lib/libpacket.a soem/wpcap/Lib/libwpcap.a soem/wpcap/Lib/Packet.lib soem/wpcap/Lib/wpcap.lib
        user/StayOpen_QMenu/stayopenmenu.h user/StayOpen_QMenu/stayopenmenu.cpp
        user/Communication/ethercatmanager.h user/Communication/ethercatmanager.cpp
        user/Information/slaveinfo.h user/Information/slaveinfo.cpp
        user/CustomTreeView/customtreeview.h user/CustomTreeView/customtreeview.cpp
    )
#***************************************************************************
target_include_directories(EasyMaster PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}/user/StayOpen_Qmenu
                           ${CMAKE_CURRENT_SOURCE_DIR}/user/CustomTreeView
                           ${CMAKE_CURRENT_SOURCE_DIR}/user/Information
                           ${CMAKE_CURRENT_SOURCE_DIR}/user/Communication
)
#***************************************************************************
    # Define target properties for Android with Qt 6 as:
#    set_property(TARGET EasyMaster APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(EasyMaster SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(EasyMaster
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(EasyMaster PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

#****************************** SOEM ********************************************
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/soem/soem_h
                    ${CMAKE_CURRENT_SOURCE_DIR}/soem/wpcap/Include
                    ${CMAKE_CURRENT_SOURCE_DIR}/soem/wpcap/Include/pcap)

target_link_libraries(EasyMaster PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/soem/lib/soem.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/soem/wpcap/Lib/x64/Packet.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/soem/wpcap/Lib/x64/wpcap.lib
#    ${CMAKE_CURRENT_SOURCE_DIR}/soem/wpcap/Lib/Packet.lib
#    ${CMAKE_CURRENT_SOURCE_DIR}/soem/wpcap/Lib/wpcap.lib
    Winmm # 链接Winmm库
    ws2_32 # 链接ws2_32.lib库
)

target_include_directories(EasyMaster PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/soem/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/soem/wpcap/Include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_definitions(-D__STDC_LIMIT_MACROS)
add_definitions(-D__STDC_CONSTANT_MACROS)
#****************************** end of SOEM ***************************************

set_target_properties(EasyMaster PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS EasyMaster
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(EasyMaster)
endif()

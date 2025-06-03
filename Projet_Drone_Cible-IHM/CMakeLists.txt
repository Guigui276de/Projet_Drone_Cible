cmake_minimum_required(VERSION 3.22)
project(projet LANGUAGES CXX)

# Trouver les bibliothèques nécessaires
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets SerialPort WebEngineWidgets)
find_package(SDL2 REQUIRED)
find_package(Fontconfig REQUIRED)

# Configuration de Qt
qt_standard_project_setup()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Définir la variable d'environnement FONTCONFIG_FILE
set(ENV{FONTCONFIG_FILE} "/etc/fonts/fonts.conf")

# Ajouter le fichier de ressources
qt6_add_resources(RESSOURCES resources.qrc)

# Ajouter l'exécutable
qt_add_executable(projet
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ${RESSOURCES}
)

# Inclure les répertoires nécessaires
include_directories(${CMAKE_SOURCE_DIR}/external)
include_directories(${SDL2_INCLUDE_DIRS})
# Lier les bibliothèques
target_link_libraries(projet
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::SerialPort
        Qt6::WebEngineWidgets
        ${SDL2_LIBRARIES}
)

# Installation et déploiement
include(GNUInstallDirs)

install(TARGETS projet
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET projet
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})

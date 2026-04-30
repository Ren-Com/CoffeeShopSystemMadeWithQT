cmake_minimum_required(VERSION 3.19)
project(CoffeeShopSystemMadeWithQT LANGUAGES CXX)

# Pastikan Charts ada di sini
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Charts)

qt_standard_project_setup()

qt_add_executable(CoffeeShopSystemMadeWithQT
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    dialog2.h dialog2.cpp dialog2.ui
    coffee.h coffee.cpp
    csvreader.h csvreader.cpp
    coffeetablemodel.h coffeetablemodel.cpp
)

target_link_libraries(CoffeeShopSystemMadeWithQT
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Charts
)

install(TARGETS CoffeeShopSystemMadeWithQT
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET CoffeeShopSystemMadeWithQT
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
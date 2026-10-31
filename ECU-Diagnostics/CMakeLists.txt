cmake_minimum_required(VERSION 3.21)

project(ECUDiagnostics VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(ECUDiagnostics
    main.cpp
    database/DatabaseManager.cpp
    database/DatabaseManager.h
)

qt_add_qml_module(ECUDiagnostics
    URI ECUDiagnostics
    VERSION 1.0
    QML_FILES
        Main.qml
)

target_link_libraries(ECUDiagnostics
    PRIVATE Qt6::Quick
    odbc32
)
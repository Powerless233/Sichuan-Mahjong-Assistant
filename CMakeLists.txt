cmake_minimum_required(VERSION 3.16)
project(SichuanMahjongAssistant VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Test)

set(CORE_SOURCES
    src/core/Tile.cpp
    src/core/Hand.cpp
    src/core/ShantenCalculator.cpp
    src/core/EfficiencyCalculator.cpp
    src/core/DiscardAnalyzer.cpp
)

set(ANALYZER_SOURCES
    src/analyzer/ShantenAnalyzer.cpp
    src/analyzer/EfficiencyAnalyzer.cpp
)

set(GUI_SOURCES
    src/gui/MainWindow.cpp
)

add_executable(SichuanMahjongAssistant
    src/main.cpp
    ${CORE_SOURCES}
    ${ANALYZER_SOURCES}
    ${GUI_SOURCES}
    resources/resources.qrc
)

target_include_directories(SichuanMahjongAssistant PRIVATE src)

target_link_libraries(SichuanMahjongAssistant PRIVATE Qt6::Widgets)

enable_testing()

add_executable(test_shanten
    tests/test_shanten.cpp
    ${CORE_SOURCES}
    ${ANALYZER_SOURCES}
)

target_include_directories(test_shanten PRIVATE src)
target_link_libraries(test_shanten PRIVATE Qt6::Test Qt6::Widgets)

add_test(NAME ShantenTests COMMAND test_shanten)

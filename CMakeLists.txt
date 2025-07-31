cmake_minimum_required(VERSION 3.30)
project(UyghurToolKit)
set(CMAKE_CXX_STANDARD 20)

include(GNUInstallDirs)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)


set(SOURCES
        src/resources/dictionaries/scripts.cpp
        src/resources/dictionaries/letter_normalize.cpp
        src/processing/processor/default_processors.cpp
        src/tools/unicode_string_convert.cpp
        src/tools/number.cpp
        src/tools/text.cpp
        src/tools/date.cpp
)
if(BUILD_SHARED_LIBS)
    add_library(core SHARED ${SOURCES})
    target_compile_definitions(core PRIVATE DLL_EXPORT)
    target_compile_definitions(core PRIVATE CORE_EXPORT)
else()
    message(STATUS "static sss")
    add_library(core STATIC ${SOURCES})
endif()

target_include_directories(core PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

install(TARGETS core
        EXPORT coreTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
)


install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

add_executable(UyghurToolKit main.cpp)
target_link_libraries(UyghurToolKit core)

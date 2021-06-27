cmake_minimum_required(VERSION 3.0.0)
Project(CARG)

set(CMAKE_CXX_STANDARD 17)

# place all predefined target projects into a filter
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
set(ARCH "x64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
set(ARCH "x86")
else()
message(STATUS "unknown architecture")
endif()

add_library(${PROJECT_NAME} STATIC
    include/carg.h
    include/ArgParser.h
    include/Exceptions.h
    include/pch.h

    src/ArgParser.cpp
    src/pch.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug-${ARCH}
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release-${ARCH}
)

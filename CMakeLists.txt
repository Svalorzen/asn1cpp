cmake_minimum_required (VERSION 3.0)

project (asn1cpp)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

# Add library directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# If enabled, add tests
if (MAKE_TESTS)
    include(CTest)
    add_subdirectory (${PROJECT_SOURCE_DIR}/test)
endif()

# Add and compile asn1c
include(ExternalProject)
ExternalProject_Add(asn1c
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/deps/asn1c
    CONFIGURE_COMMAND ${PROJECT_SOURCE_DIR}/deps/asn1c/configure --prefix=${PROJECT_BINARY_DIR}
    BUILD_COMMAND ${MAKE}
)

ExternalProject_Add_Step(asn1c autoreconf
     COMMAND autoreconf -iv
     WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/asn1c/
     DEPENDERS configure
)

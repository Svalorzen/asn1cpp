cmake_minimum_required (VERSION 3.3)

project (asn1cpp)

set(ASN_OPTIONS -fwide-types -gen-PER)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

# Add library directories
include_directories(${PROJECT_SOURCE_DIR}/include)

if (NOT DEFINED MAKE_TESTS)
    set(MAKE_TESTS 1)
endif()

# If enabled, add tests
if (MAKE_TESTS)
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

    include(CTest)
    add_subdirectory (${PROJECT_SOURCE_DIR}/test)
endif()

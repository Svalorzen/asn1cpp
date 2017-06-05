cmake_minimum_required (VERSION 3.3)

set(GEN_FOLDER ${PROJECT_BINARY_DIR}/gen)
set(ASN_CMAKE ${PROJECT_SOURCE_DIR}/test/build_utils/CMakeForLibs)
set(ADDITIONAL_SOURCES ${PROJECT_SOURCE_DIR}/test/build_utils/setof_encode_uper.c)

function (SetupSuite SUITE)
    set(ASN1_FILE ${PROJECT_SOURCE_DIR}/test/${SUITE}/definitions.asn1)
    set(SUITE_FOLDER ${GEN_FOLDER}/${SUITE})

    # Generating asn1 C files for this suite
    add_custom_command(OUTPUT ${SUITE_FOLDER}
        COMMAND rm -rf ${SUITE_FOLDER}
        COMMAND mkdir -p ${SUITE_FOLDER}
        COMMAND cd ${SUITE_FOLDER} && ${PROJECT_BINARY_DIR}/bin/asn1c ${ASN_OPTIONS} ${ASN1_FILE}
        COMMAND rm -f ${SUITE_FOLDER}/converter-sample.c
        COMMAND cp ${ADDITIONAL_SOURCES} ${SUITE_FOLDER}
        COMMAND cp ${ASN_CMAKE} ${SUITE_FOLDER}/CMakeLists.txt
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        DEPENDS asn1c ${ASN1_FILE} ${ASN_CMAKE}
    )
    add_custom_target(gen_${SUITE} DEPENDS ${SUITE_FOLDER})

    # Building library on the fly
    add_custom_command(OUTPUT ${SUITE_FOLDER}/build/libasn1.a
        COMMAND mkdir -p build
        COMMAND cd build && cmake .. && make
        WORKING_DIRECTORY ${SUITE_FOLDER}
        DEPENDS gen_${SUITE}
    )
    add_custom_target(libasn1 DEPENDS ${SUITE_FOLDER}/build/libasn1.a)

    # Export the library so it can be used
    add_library(${SUITE}_lib STATIC IMPORTED GLOBAL)
    add_dependencies(${SUITE}_lib libasn1)
    set_target_properties(${SUITE}_lib PROPERTIES
        IMPORTED_LOCATION ${SUITE_FOLDER}/build/libasn1.a)
endfunction (SetupSuite)

function (AddTest SUITE TEST)
    set(SUITE_FOLDER ${GEN_FOLDER}/${SUITE})
    set(EXE ${SUITE}_${TEST})
    add_executable(${EXE}Tests ./${SUITE}/${TEST}Tests.cpp)
    target_link_libraries(${EXE}Tests ${SUITE}_lib ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
    target_include_directories(${EXE}Tests PRIVATE ${SUITE_FOLDER})
    add_test(NAME ${EXE} WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COMMAND $<TARGET_FILE:${EXE}Tests>)
endfunction (AddTest)

find_package(Boost 1.53 COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

SetupSuite(common)
AddTest(common int)
AddTest(common bool)
AddTest(common string)
AddTest(common enum)
AddTest(common optional)
AddTest(common nested)
AddTest(common setof)
AddTest(common sequenceof)

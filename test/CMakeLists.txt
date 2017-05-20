cmake_minimum_required (VERSION 3.3)

set(GEN_FOLDER ${PROJECT_BINARY_DIR}/gen)

function (SetupSuite SUITE)
    set(ASN1_FILE ${PROJECT_SOURCE_DIR}/test/${SUITE}/definitions.asn1)
    set(SUITE_FOLDER ${GEN_FOLDER}/${SUITE})

    add_custom_command(OUTPUT ${SUITE_FOLDER}
        COMMAND rm -rf ${SUITE_FOLDER}
        COMMAND mkdir -p ${SUITE_FOLDER}
        COMMAND cd ${SUITE_FOLDER} && ${PROJECT_BINARY_DIR}/bin/asn1c ${ASN_OPTIONS} ${ASN1_FILE}
        COMMAND rm -f ${SUITE_FOLDER}/converter-sample.c
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        DEPENDS asn1c ${COMMON_ASN1_FILE}
    )
    add_custom_target(gen_${SUITE} DEPENDS ${SUITE_FOLDER})
endfunction (SetupSuite)

function (AddTest suite name)
    set(exename ${suite}_${name})
    add_executable(${exename}Tests ./${suite}/${name}Tests.cpp)
    add_dependencies(${exename}Tests gen_${suite})
    add_test(NAME ${exename} WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COMMAND $<TARGET_FILE:${exename}Tests>)
endfunction (AddTest)

SetupSuite(common)
AddTest(common main)

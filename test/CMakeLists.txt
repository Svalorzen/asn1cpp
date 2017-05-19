cmake_minimum_required (VERSION 3.3)

set(GEN_FOLDER ${PROJECT_BINARY_DIR}/gen)

# Adding data dependency for common tests
set(COMMON_ASN1_FILE ${PROJECT_SOURCE_DIR}/test/common/definitions.asn1)

add_custom_command(OUTPUT ${GEN_FOLDER}/common
    COMMAND mkdir -p ${GEN_FOLDER}/common/
    COMMAND cd ${GEN_FOLDER}/common && ${PROJECT_BINARY_DIR}/bin/asn1c ${ASN_OPTIONS} ${COMMON_ASN1_FILE}
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    DEPENDS asn1c ${COMMON_ASN1_FILE}
)
add_custom_target(gen_common DEPENDS ${PROJECT_BINARY_DIR}/gen/common)

function (AddTestCommon name)
    set(exename Common_${name})
    add_executable(${exename}Tests ./common/${name}Tests.cpp)
    add_dependencies(${exename}Tests gen_common)
    add_test(NAME ${exename} WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COMMAND $<TARGET_FILE:${exename}Tests>)
endfunction (AddTestCommon)

AddTestCommon(main)

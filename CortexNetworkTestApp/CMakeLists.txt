include("${PROJECT_SOURCE_DIR}/CMake/Target/CortexM4F.cmake")
include("_Sources.cmake")

set(CMAKE_UNITY_BUILD true)
set(CMAKE_UNITY_BUILD_BATCH_SIZE 16)

set(EXE_NAME CortexNetworkTestApp)

set(LINKER_SCRIPT "\"${CMAKE_CURRENT_SOURCE_DIR}/LinkerScript.ld\"")
Target_CortexM4F(${EXE_NAME})

set(VENDOR_DIR ${PROJECT_SOURCE_DIR}/${EXE_NAME}/vendor)

add_compile_options(
    ${CPU_OPTIONS}
    $<$<COMPILE_LANGUAGE:CXX>:-std=c++17>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
    $<$<COMPILE_LANGUAGE:CXX>:-Wsuggest-override>
    $<$<COMPILE_LANGUAGE:CXX>:-Wno-old-style-cast>
    
    -Wl,--gc-sections # remove unused sections
    
    -Wall
    -Wextra
    -Wcast-align
    -Wsign-conversion
    -Wshadow
    -Wlogical-op
    -Wsuggest-final-types
    -Wsuggest-final-methods
    -Wno-unused-parameter
    -Wno-unused-function
    -Wno-sign-conversion
    -Wno-narrowing
    -Wno-sign-compare
    -Wno-shift-negative-value
    -Wno-cast-align
    -Wno-write-strings
    -Wno-tautological-compare

    $<$<COMPILE_LANGUAGE:CXX>:-Wno-literal-suffix>
    $<$<COMPILE_LANGUAGE:CXX>:-Wno-register>
    $<$<COMPILE_LANGUAGE:CXX>:-fpermissive>
    

    #-fdebug-prefix-map="${CMAKE_SOURCE_DIR}"=.
)

add_executable(${EXE_NAME} ${sources} ${headers})

target_include_directories(${EXE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${EXE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/_config/lwip)
include_directories(${EXE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/_config/lwip/include)
include("_Modules.cmake")

target_compile_definitions(${EXE_NAME} PUBLIC PART_TM4C1294NCPDT TARGET_IS_TM4C129_RA2)

target_compile_options(${EXE_NAME} PRIVATE "-flto")

target_precompile_headers(
    ${EXE_NAME}
    PUBLIC
    <stddef.h>
    <stdint.h>
    <stdio.h>
    <stdlib.h>
    <stdbool.h>
    <string.h>
    <CFXS.hpp>
)

target_link_options(
    ${EXE_NAME}
    PRIVATE
    ${CPU_OPTIONS}
    SHELL:-T${LINKER_SCRIPT}
    -Xlinker -Map=output.map
)

add_custom_command(TARGET ${EXE_NAME} POST_BUILD
    COMMAND objcopy -S -O binary ${CMAKE_CURRENT_SOURCE_DIR}/../build/${EXE_NAME}/${EXE_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../build/${EXE_NAME}.bin)
add_custom_command(TARGET ${EXE_NAME} POST_BUILD
    COMMAND nm --print-size --size-sort --format=bsd --radix=d ${CMAKE_CURRENT_SOURCE_DIR}/../build/${EXE_NAME}/${EXE_NAME} > ${CMAKE_CURRENT_SOURCE_DIR}/../build/${EXE_NAME}_Symbols.txt)
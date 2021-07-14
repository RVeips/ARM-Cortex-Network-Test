include("${PROJECT_SOURCE_DIR}/CMake/Target/CortexM4F.cmake")
include("_Sources.cmake")

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
    
    -fpermissive

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

    #-fdebug-prefix-map="${CMAKE_SOURCE_DIR}"=.
)

add_executable(${EXE_NAME} ${sources} ${headers})

target_include_directories(${EXE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
include("_Modules.cmake")

target_compile_definitions(${EXE_NAME} PUBLIC PART_TM4C1294NCPDT TARGET_IS_TM4C129_RA2)


target_precompile_headers(
    ${EXE_NAME}
    PUBLIC
    <cstddef>
    <cstdint>
    <cstdio>
    <cstdlib>
    <cstdbool>
    <cstring>
    <preinclude.hpp>
    "$<$<COMPILE_LANGUAGE:CXX>:<vector$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<map$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<unordered_map$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<utility$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<memory$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<algorithm$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<array$<ANGLE-R>>"
)

target_link_libraries(${EXE_NAME}
    ${CPU_OPTIONS}
    -T${LINKER_SCRIPT}
    -Xlinker -Map=output.map
    -o${EXE_NAME}.elf.
)

add_custom_command(TARGET ${EXE_NAME} POST_BUILD
    COMMAND objcopy -S -O binary ${CMAKE_CURRENT_SOURCE_DIR}/../build/${EXE_NAME}.elf ${CMAKE_CURRENT_SOURCE_DIR}/../build/${EXE_NAME}.bin)
add_custom_command(TARGET ${EXE_NAME} POST_BUILD
    COMMAND nm --print-size --size-sort --format=bsd --radix=d ${CMAKE_CURRENT_SOURCE_DIR}/../build/${EXE_NAME}.elf > ${CMAKE_CURRENT_SOURCE_DIR}/../build/${EXE_NAME}_Symbols.txt)
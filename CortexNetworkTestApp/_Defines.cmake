# target
target_compile_definitions(${EXE_NAME} PUBLIC "PART_TM4C1294NCPDT" "TARGET_IS_TM4C129_RA2")

#terminal
target_compile_definitions(${EXE_NAME} PUBLIC "CFXS_DEBUG_TERMINAL_USE_COLORS")

# debug
target_compile_definitions(${EXE_NAME} PUBLIC "DEBUG")
target_compile_definitions(${EXE_NAME} PUBLIC "CFXS_DEBUG" "CFXS_DEBUG_ASSERT")
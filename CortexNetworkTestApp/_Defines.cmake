# target
add_compile_definitions("PART_TM4C1294NCPDT" "TARGET_IS_TM4C129_RA2")
add_compile_definitions("CFXS_PLATFORM_TIVA")

# target network
add_compile_definitions("PART_TM4C1294NCPDT" "TARGET_IS_TM4C129_RA2")
add_compile_definitions("CFXS_IP_IPv4")
add_compile_definitions("CFXS_IP_HW_PHY_INTERNAL" "CFXS_IP_HW_TX_DESCRIPTORS=16" "CFXS_IP_HW_RX_DESCRIPTORS=16")

#terminal
add_compile_definitions("CFXS_DEBUG_TERMINAL_USE_COLORS")

# debug
add_compile_definitions("DEBUG")
add_compile_definitions("CFXS_DEBUG" "CFXS_DEBUG_ASSERT")
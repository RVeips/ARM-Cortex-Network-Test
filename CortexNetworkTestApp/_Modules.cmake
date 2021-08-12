# Segger RTT printf implementation
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/SeggerRTT")
target_link_libraries(${EXE_NAME} PUBLIC printf_impl_SeggerRTT)

# driverlib
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/tm4c-driverlib")
target_link_libraries(${EXE_NAME} PUBLIC TM4C_driverlib)

# lwip
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/lwip")
target_link_libraries(${EXE_NAME} PUBLIC lwip)

# CFXS Base
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/CFXS-Base")
target_link_libraries(${EXE_NAME} PUBLIC CFXS_Base)

# CFXS IP
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/CFXS-IP")
target_include_directories(CFXS_IP PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/CFXS-Base/include")
target_compile_definitions(CFXS_IP PUBLIC "CFXS_IP_IPv4")
target_link_libraries(${EXE_NAME} PUBLIC CFXS_IP)

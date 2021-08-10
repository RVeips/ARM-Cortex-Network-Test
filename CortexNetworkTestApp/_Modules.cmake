# Segger RTT printf implementation
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/SeggerRTT")
target_link_libraries(${EXE_NAME} PRIVATE printf_impl_SeggerRTT)

# driverlib
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/tm4c-driverlib")
target_link_libraries(${EXE_NAME} PRIVATE TM4C_driverlib)

# lwip
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/lwip")
target_link_libraries(${EXE_NAME} PRIVATE lwip)

# CFXS Base
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/CFXS-Base")
target_link_libraries(${EXE_NAME} PRIVATE CFXS_Base)
# driverlib
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/tm4c-driverlib")
target_link_libraries(${EXE_NAME} PRIVATE TM4C_driverlib)

# lwip
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/lwip")
target_link_libraries(${EXE_NAME} PRIVATE lwip)
include_directories (.)

add_library (revolution SHARED ${HEADERS} ${SOURCES} )
set_target_properties(revolution PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${COMMON_OUTPUT_DIR}
    SOVERSION 1.1
    VERSION 1.1)
install (TARGETS revolution DESTINATION "${PROJECT_BINARY_DIR}/lib")
install (FILES revolution.h DESTINATION "${PROJECT_BINARY_DIR}/include")

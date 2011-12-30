include_directories (.)

SET (rev_src 
		version.cpp 
		version.h
		revolution.cpp
		revolution.h
		objective_function.cpp
		objective_function.h
		basic_es.h
		basic_es.cpp
		atom.h
		atom.cpp
		population.h
)

add_library (revolution SHARED ${rev_src})
set_target_properties(revolution PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${COMMON_OUTPUT_DIR}
    SOVERSION 1.0
    VERSION 1.0)
install (TARGETS revolution DESTINATION "${PROJECT_BINARY_DIR}/lib")
install (FILES revolution.h DESTINATION "${PROJECT_BINARY_DIR}/include")

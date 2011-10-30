include_directories (.)

SET (rev_src 
		version.cpp 
		version.h
		revolution.cpp
		revolution.h
		objective_function.cpp
		objective_function.h
)

add_library (revolution SHARED ${rev_src})
install (TARGETS revolution DESTINATION "${PROJECT_BINARY_DIR}/lib")
install (FILES revolution.h DESTINATION "${PROJECT_BINARY_DIR}/include")

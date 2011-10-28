SET (rev_src 
		version.cpp 
		revolution.cpp
		objective_function.cpp
)

add_library (revolution SHARED ${rev_src})

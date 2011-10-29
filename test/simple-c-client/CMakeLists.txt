set (simple_test_c_src main.c)

add_executable (main_c ${simple_test_c_src})

target_link_libraries (main_c revolution)

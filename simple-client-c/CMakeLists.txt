set (simple_test_src main.c)

add_executable (main ${simple_test_src})

target_link_libraries (main revolution)

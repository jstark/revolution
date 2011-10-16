set (simple_test_src main.cpp)

add_executable (main ${simple_test_src})

target_link_libraries (main revolution)

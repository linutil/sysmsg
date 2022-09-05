project(sysmsg)

add_executable(sysmsg main.c)
target_link_libraries(sysmsg uuid)
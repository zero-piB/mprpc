

# =========== 宏库 ==========
set(assert_test assert_test.cc)
set(common_lib 
    dl
)
add_executable(assert_test ${assert_test})

target_link_libraries(assert_test
    ${common_lib}
)

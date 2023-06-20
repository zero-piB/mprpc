

set(common_lib 
    dl
)
# =========== 宏库test ==========
set(assert_test assert_test.cc)

add_executable(assert_test ${assert_test})

target_link_libraries(assert_test
    ${common_lib}
)

# =========== fiber test ==========
add_executable(fiber_test 
    fiber_test.cc
    ../src/common/fiber.cc
)

# target_link_libraries(fiber_test
#     ${common_lib}
# )


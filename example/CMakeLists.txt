
add_subdirectory(user)

set(logger_test logger_test.cc)

add_executable(logger_test ${logger_test})

target_link_libraries(logger_test
    pthread
    dl
)

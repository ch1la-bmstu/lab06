add_executable(check
    test_account.cpp
    test_transaction.cpp
)

target_link_libraries(check banking gtest_main)

add_test(NAME check COMMAND check)

set(SNAKE_TEST_SOURCE_LIST "")

add_executable(ttest test.c)

include(snake/snake.cmake)

create_test_sourcelist(TEST_SOURCE test.c ${SNAKE_TEST_SOURCE_LIST})

target_sources(ttest PRIVATE ${SNAKE_TEST_SOURCE_LIST})

include(util/util.cmake)

#add libraries
target_link_libraries(ttest PRIVATE snake_game)
target_link_libraries(ttest PRIVATE global_interface)

foreach(test ${SNAKE_TEST_SOURCE_LIST})
string(REPLACE ".c" "" command ${test})
string(REPLACE "/" "_" name ${command})
string(TOUPPER ${name} name)
add_test(NAME ${name} COMMAND ttest ${command} WORKING_DIRECTORY ${TEST_ENV})
endforeach()

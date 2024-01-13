file(GLOB SNAKE_C_SOURCE CONFIGURE_DEPENDS *.c)

add_library(snake_game ${SNAKE_C_SOURCE})

target_link_libraries(snake_game PRIVATE global_interface)
target_include_directories(snake_game PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)


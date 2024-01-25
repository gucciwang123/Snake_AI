file(GLOB UTIL_C_SOURCE CONFIGURE_DEPENDS util/*.c)

target_sources(ttest PRIVATE ${UTIL_C_SOURCE})
target_include_directories(ttest PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)

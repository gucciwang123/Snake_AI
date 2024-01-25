file(GLOB SNAKE_TEST_C_SOURCE_FULL CONFIGURE_DEPENDS snake/*.c)

foreach(file ${SNAKE_TEST_C_SOURCE_FULL})
file(RELATIVE_PATH source ${CMAKE_CURRENT_SOURCE_DIR} ${file})
list(APPEND SNAKE_TEST_SOURCE_LIST ${source})
endforeach()

file(GLOB_RECURSE SNAKE_TEST_ASSETS CONFIGURE_DEPENDS snake/assets/*.sn_game)

list(LENGTH SNAKE_TEST_ASSETS length)

if(NOT length EQUAL 0)
	set(filenames "")
	set(byproducts "")
	foreach(file ${SNAKE_TEST_ASSETS})
		get_filename_component(filename ${file} NAME)
		list(APPEND filenames ${filename})
		list(APPEND byproducts ${TEST_ENV}/${filename})
	endforeach()

	add_custom_target(
		test-assets
		ALL
		BYPRODUCTS ${byproducts}
		COMMAND cp ${SNAKE_TEST_ASSETS} ${TEST_ENV}
		)

	add_dependencies(ttest test-assets)
endif()

file(GLOB AMD64_SO_FILES *.so *.so.*)
file(GLOB AMD64_A_FILES *.a)

# Make the files visible on qtcreator
add_custom_target(AMD64_SO_LIBS ALL SOURCES ${AMD64_SO_FILES})
add_custom_target(AMD64_A_LIBS ALL SOURCES ${AMD64_A_FILES})

install(FILES ${AMD64_SO_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}lib/x86_64)

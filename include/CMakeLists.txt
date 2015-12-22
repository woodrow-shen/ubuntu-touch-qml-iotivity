file(GLOB OCLIB_INCLUDE_FILES OCLIB/*.h)

# Make the files visible on qtcreator
add_custom_target(OCLIB_INCLUDES ALL SOURCES ${OSLIB_INCLUDE_FILES})

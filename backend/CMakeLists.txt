if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++11)
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
)

#For the static library:
#set ( PROJECT_LINK_LIBS liboctbstack.a )

#For the shared library:
set (
    PROJECT_LINK_LIBS
    liboc.so
    liboc_logger.so
    liboctbstack.so
)

link_directories( ${CMAKE_SOURCE_DIR}/lib/arm-linux-gnueabihf )
include_directories(${CMAKE_SOURCE_DIR}/include)

set(
    SimpleCbackend_SRCS
    modules/SimpleC/backend.cpp
    modules/SimpleC/mytype.cpp
)

# Make the unit test files visible on qtcreator
add_custom_target(SimpleCbackend_UNITTEST_QML_FILES ALL SOURCES "tests/unit/tst_mytype.qml")

add_library(SimpleCbackend MODULE
    ${SimpleCbackend_SRCS}
)

target_link_libraries(SimpleCbackend ${PROJECT_LINK_LIBS})

set_target_properties(SimpleCbackend PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY SimpleC)

qt5_use_modules(SimpleCbackend Gui Qml Quick)

# Copy qmldir file to build dir for running in QtCreator
add_custom_target(SimpleCbackend-qmldir ALL
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/modules/SimpleC/qmldir ${CMAKE_CURRENT_BINARY_DIR}/SimpleC
    DEPENDS ${QMLFILES}
)

# Install plugin file
install(TARGETS SimpleCbackend DESTINATION ${QT_IMPORTS_DIR}/SimpleC/)
install(FILES   modules/SimpleC/qmldir DESTINATION ${QT_IMPORTS_DIR}/SimpleC/)
#install(FILES   iotivity-multi-lib/${ARCH}/liboc.so DESTINATION )


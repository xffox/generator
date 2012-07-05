cmake_minimum_required(VERSION 2.6)
include(FindPkgConfig)

pkg_search_module(CPPUNIT cppunit)
if(CPPUNIT_FOUND)
    include_directories("${PROJECT_SOURCE_DIR}/generator/include")

    set(TARGET_NAME generator_test)

    set(GENERATOR_TEST_SRCS test.cpp
        GeneratorTest.cpp)

    add_executable(${TARGET_NAME} ${GENERATOR_TEST_SRCS})
    target_link_libraries(${TARGET_NAME} ${CPPUNIT_LIBRARIES} pthread)

    get_target_property(GENERATOR_TEST_PATH ${TARGET_NAME} LOCATION)
    add_test(UtilTest "${GENERATOR_TEST_PATH}")
else(CPPUNIT_FOUND)
    message(WARNING "cppunit not found - testing is not available")
endif(CPPUNIT_FOUND)

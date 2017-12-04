cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)
project(runUnitTests)

find_package(GTest REQUIRED)
find_package( OpenCV )
find_package( Boost COMPONENTS system filesystem REQUIRED)
if (Boost_FOUND)
    INCLUDE_DIRECTORIES( SYSTEM ${Boost_INCLUDE_DIR} )
    ADD_DEFINITIONS( "-DHAS_BOOST" )
endif()

include_directories(${GTEST_INCLUDE_DIRS})
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(lib)
include_directories(../include)

set(test_SRCS main.cpp
        indices_mapping/IndicesMappingTest.cpp
        utils/UtilsTest.cpp
        utils/utils.cpp)

add_executable(runUnitTests ${test_SRCS})

add_subdirectory(indices_mapping)
#add_subdirectory(phash)
#add_subdirectory(surf)
add_subdirectory(utils)

target_link_libraries(runUnitTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runUnitTests ${OpenCV_LIBS} )
target_link_libraries(runUnitTests
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY})
enable_testing()
add_test(NAME runUnitTests COMMAND runUnitTests)
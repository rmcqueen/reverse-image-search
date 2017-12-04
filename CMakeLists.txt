cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)
project(reverse-image-search )

find_package( OpenCV )
find_package( Boost COMPONENTS system filesystem REQUIRED)
if (Boost_FOUND)
    INCLUDE_DIRECTORIES( SYSTEM ${Boost_INCLUDE_DIR} )
    ADD_DEFINITIONS( "-DHAS_BOOST" )
endif()

include_directories(include)
include_directories(data)
add_subdirectory(src)
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(lib)
#add_subdirectory(tests)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(SOURCE_FILES src/main.cpp
        src/Surf.cpp
        src/utils.cpp
        include/Surf.hpp
        include/utils.hpp
        include/Vocabulary.hpp
        include/Histogram.hpp
        src/Histogram.cpp
        src/Vocabulary.cpp
        src/SVM.cpp
        include/SVM.hpp)

add_executable(reverse-image-search ${SOURCE_FILES})

link_libraries(core_lib)
link_libraries(core)
target_link_libraries(reverse-image-search ${OpenCV_LIBS} )
target_link_libraries(reverse-image-search
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY})
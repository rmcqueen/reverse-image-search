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
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(lib)
include_directories(test)


set(IMAGE_HASH_FILES
        include/img_hash_opencv_module/phash.cpp
        include/img_hash_opencv_module/PHash_Fast.cpp)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
add_executable(reverse-image-search
        include/PerceptualHash.h
        include/Surf.h
        include/utils.h
        include/Image.h
        include/IndicesMapping.h
        src/Surf.cpp
        src/main.cpp
        src/utils.cpp)

link_libraries(lib)
target_link_libraries(reverse-image-search ${OpenCV_LIBS} )
target_link_libraries(reverse-image-search ${CMAKE_CURRENT_SOURCE_DIR}/lib/hiredis/libhiredis.a)
target_link_libraries(reverse-image-search
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY})
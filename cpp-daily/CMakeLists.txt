cmake_minimum_required(VERSION 3.9)
project(cpp-daily)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost COMPONENTS system filesystem REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif(Boost_FOUND)

set(SOURCE_FILES boost/asio/buffer.cpp)
add_executable(cpp-daily ${SOURCE_FILES})

set(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})
target_link_libraries(cpp-daily ${USED_LIBS})


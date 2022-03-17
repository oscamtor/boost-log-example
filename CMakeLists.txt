cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project(boost_log CXX)

set(CMAKE_CXX_STANDARD 14)

set(BOOST_ROOT "boost_1_71")

set(Boost_INCLUDE_DIR /usr/include/)
set(Boost_LIBRARY_DIR /usr/lib/x86_64-linux-gnu/)

# 4. Add DBOOST_LOG_DYN_LINK into CMAKE_CXX_FLAGS flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fPIC -fopenmp  -DBOOST_LOG_DYN_LINK -std=c++11 -pthread")

# we will use static libs
#set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
#set(Boost_USE_STATIC_LIBS ON)

find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package ( Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED )
#find_package(Boost REQUIRED COMPONENTS regex)
find_package(Boost 1.71.0 COMPONENTS thread log log_setup)


set (Boost_DEBUG ON )
if(Boost_FOUND)
    message(STATUS "Success!")
endif()
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG=1)
endif()

link_directories("/usr/lib/x86_64-linux-gnu/")
# For librt
link_directories("/lib/x86_64-linux-gnu/")

#set (Boost_USE_STATIC_LIBS ON) # link statically
ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)  # or, link dynamically

#set ( BOOST_ASIO_NO_DEPRECATED ON )
set ( BOOST_COMPONENTS system )
find_package ( Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED )
foreach ( LIBRARY ${BOOST_COMPONENTS} )
	list ( APPEND BOOST_LIBRARIES Boost::${LIBRARY} )
endforeach ()

include_directories(${Boost_INCLUDE_DIR})


# 2. Append log and log_setup into other libs:
set(PROJECT_LIB ${PROJECT_LIB} ${Boost_LOG_LIBRARY} ${Boost_LOG_SETUP_LIBRARY})

find_package(Threads)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

set(SRC_FILES
    src/main.cpp)

add_executable(boost_log ${SRC_FILES})

#target_link_libraries(boost_log ${Boost_LIBRARIES} Threads::Threads)
# 3. Link those libraries into your program,
# To avoid undefined reference to `boost::log::v2_mt_posix...
target_link_libraries(${PROJECT_NAME} -Wl,--start-group  ${PROJECT_LIB} -Wl,--end-group
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  pthread Boost::log Boost::log_setup
)

find_library(LIBRT rt)
if(LIBRT)
    target_link_libraries(${PROJECT_NAME} ${LIBRT})
endif()
















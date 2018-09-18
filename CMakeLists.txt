project(cub C CXX)

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)

if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-invalid-offsetof")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE cub_srcs
  "*.cc"
)

file(GLOB_RECURSE cub_test_srcs
  "*Test.cc"
)
list(REMOVE_ITEM cub_srcs ${cub_test_srcs})

if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

add_library(cub STATIC ${cub_srcs})

if(ENABLE_TEST)
  add_executable(cub-test ${cub_test_srcs} ${main_src})
  target_link_libraries(cub-test cub cut-main cut)
endif()

install(DIRECTORY cub/ DESTINATION include/cub
        FILES_MATCHING PATTERN "*.h")
install(TARGETS cub ARCHIVE DESTINATION lib)

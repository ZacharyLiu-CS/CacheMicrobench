#1. specify the version of cmake
cmake_minimum_required(VERSION 3.10)

#2. define the name of project
project(microbench)
# export compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-O3 -g -Werror -Wall -march=cascadelake -mtune=skylake-avx512 -mavx -mavx2)

#2.1 update git submodules
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
  execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE GIT_SUBMOD_RESULT)
  if(NOT GIT_SUBMOD_RESULT EQUAL "0")
    message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
  endif()
endif()

#3. specify c++ compiler version
set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -pthread")

find_package(gflags REQUIRED)


#4. head file path
include_directories(
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/include
  )

#5. executable source code file path
file(GLOB MAIN_FILE
  ${PROJECT_SOURCE_DIR}/src/*.cc
  )
#6. other source code file
file(GLOB SOURCE_FILE
  ${PROJECT_SOURCE_DIR}/utils/*.cc
  )

#7. config each executable binary file
foreach(testsourcefile ${MAIN_FILE})
  string(REGEX MATCH "[^/]+$" testsourcefilewithoutpath ${testsourcefile})
  string(REPLACE ".cc" "" testname ${testsourcefilewithoutpath})
  add_executable( ${testname} ${testsourcefile} ${SOURCE_FILE})
  target_link_libraries(${testname}
    gflags
    pthread
    )
ENDFOREACH(testsourcefile ${TEST_FILE})

#8. soft link compile_commands to 'build' path
if(EXISTS "${PROJECT_SOURCE_DIR}/compile_commands.json")
  execute_process(
    COMMAND rm ${PROJECT_SOURCE_DIR}/compile_commands.json
    )
endif()
execute_process(
  COMMAND ln -s -f ${PROJECT_SOURCE_DIR}/build/compile_commands.json ${PROJECT_SOURCE_DIR}/compile_commands.json
  )

